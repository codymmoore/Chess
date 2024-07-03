#include "agent.h"

// Typedefs
typedef std::chrono::high_resolution_clock::time_point chronoTime;
typedef std::chrono::duration<std::chrono::nanoseconds> chronoDuration;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  squash_function(const int value)
///
/// \brief:  Normalize positive integers between 0 and 1
///
/// \param [in]:  value : positive integer to be normalized
///
/// \return:  double : normalized value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double squash_function(const int value)
{
	return (1 / (1 + pow(SQUASH_COEFFICIENT, static_cast<double>(-value))));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  int Agent::pieceValueHeuristic(const ChessState& game) const
///
/// \brief:  Provide heuristic value for a game state based on piece values for each player
///
/// \param [in]:  game : game state to be evaluated
///
/// \return:  int : resulting heuristic value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Agent::pieceValueHeuristic(const ChessState& game) const
{
	int result = 0;
	const std::vector<PieceNode>* allies, * enemies;

	if (m_player == WHITE)
	{
		allies = &game.m_whitePieces;
		enemies = &game.m_blackPieces;
	}
	else
	{
		allies = &game.m_blackPieces;
		enemies = &game.m_whitePieces;
	}

	// Calculate sum of point values for each player-controlled piece
	for (const PieceNode& piece : *allies)
	{
		result += PIECE_VALUES[piece.m_pieceType];
	}

	int capturePoints = FULL_TEAM_VALUE;

	// Add values for captured enemy pieces
	for (const PieceNode& piece : *enemies)
	{
		capturePoints -= PIECE_VALUES[piece.m_pieceType];
	}

	result += capturePoints;

	result += Move::inCheck(~m_player, game) * CHECK_VALUE;
	result -= Move::inCheck(m_player, game) * CHECK_VALUE;

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool Move::timeHeuristic(const double timeElapsed, const double timeRemaining) const
///
/// \brief:  Indicate whether algorithm should keep searching given the time elapsed for the current player's
///             turn and the total time remaining
///
/// \param [in]:  timeElapsed  : current time elapsed since start of turn
/// \param [in]:  timeRemaining : time remaining until timeout
///
/// \return:  bool : true if algorithm should keep searching, otherwise false
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Agent::timeHeuristic(const double timeElapsed, const double timeRemaining) const
{
	double turnTime = 0.01 * timeRemaining;

	return timeElapsed < turnTime;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  bool Agent::isQuiescent(const ChessState& game, const std::unordered_map<Position, std::vector<Position>, 
///         PositionHasher>& validMoves) const
///
/// \brief:  Indicate whether a state is likely to exhibit a large change in value
///
/// \param [in]:  game : state that is being evaluated
/// \param [in]:  validMoves : contains valid moves for each piece under player's control
///
/// \return:  bool : true if game state is quiescent, false otherwise
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Agent::isQuiescent(const ChessState& game, const std::unordered_map<Position, std::vector<Position>, PositionHasher>& validMoves) const
{
	bool result = true;
	std::unordered_map<Position, std::vector<Position>, PositionHasher>::const_iterator pieceMoves = validMoves.begin(),
		pieceMovesEnd = validMoves.end();
	// Loops through each piece with valid moves
	while (pieceMoves != pieceMovesEnd && result)
	{
		std::vector<Position>::const_iterator move = pieceMoves->second.begin(),
			movesEnd = pieceMoves->second.end();

		// Loops through each valid move for current piece
		while (move != movesEnd && result)
		{
			// Set result to true if a capture occurs
			result = !game.m_board.posIsOccupied(*move);
			move++;
		}
		pieceMoves++;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  double Agent::getMoveValue(const Color& player, const Position& prevPos, const Position& newPos, 
///         const ChessState& game) const
///
/// \brief:  - Assign a move an integer value given its ending position
///          - Will always assume move is valid
///
/// \param [in]:  player : color player who is performing move
/// \param [in]:  prevPos : original position of piece that is being moved
/// \param [in]:  newPos : position that piece is being moved to
/// \param [in]:  game : game state in which move is to be performed
///
/// \return:  double : resulting move value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double Agent::getMoveValue(const Color& player, const Position& prevPos, const Position& newPos, const ChessState& game) const
{
	int result = 1, pieceType = PAWN;
	bool occupied = false;

	// Determine if a capture occurs
	while (pieceType < KING && !occupied)
	{
		occupied = game.m_board.posIsOccupiedByColorPiece(newPos, ~player, (PieceType)pieceType);

		pieceType += 1;
	}

	if (occupied)
	{
		// If a piece is captured, add corresponding value to result
		result += PIECE_VALUES[pieceType - 1];
	}
	else // if no capture occurs
	{
		int deltaY = newPos.m_y - prevPos.m_y;

		// Subtract one from result if piece is moving backwards
		result -= ((deltaY > 0 && player == WHITE) || (deltaY < 0 && player == BLACK));
	}

	return static_cast<double>(result);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::vector<std::pair<Position, Position>> Agent::orderMoves(const Color& player, 
///         const std::unordered_map<Position, std::vector<Position>, PositionHasher>& validMoves, 
///         const ChessState& game) const
///
/// \brief:  Order list of moves according to values assigned by getMoveValue()
///             and player's corresponding history table
///
/// \param [in]:  player : color of player who is making move
/// \param [in]:  validMoves : contains valid moves for each player-controlled piece
/// \param [in]:  game : game state in which moves are to be performed
///
/// \return:  vector<pair<Position, Position>> : reordered moves
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<std::pair<Position, Position>> Agent::orderMoves(const Color& player, const std::unordered_map<Position, std::vector<Position>, PositionHasher>& validMoves, const ChessState& game) const
{
	std::multimap<double, std::pair<Position, Position>, std::greater<double>> moveSorter;
	const std::unordered_map<Position, std::unordered_map<Position, int, PositionHasher>, PositionHasher>&
		historyTable = (player == m_player ? m_allyHistoryTable : m_enemyHistoryTable);
	std::vector<std::pair<Position, Position>> result;

	// Loop through each piece with valid moves
	for (const std::pair<Position, std::vector<Position>>& pieceMoves : validMoves)
	{
		Position prevPos = pieceMoves.first;

		// Loop through each valid move for current piece
		for (const Position& destination : pieceMoves.second)
		{
			double moveValue = getMoveValue(player, prevPos, destination, game);
			double historyTableValue = 0.0;

			// Set history table value if move exists in history table
			const std::unordered_map<Position, std::unordered_map<Position, int, PositionHasher>, PositionHasher>::const_iterator
				prevPosIt = historyTable.find(prevPos);

			if (prevPosIt != historyTable.end())
			{
				std::unordered_map<Position, int, PositionHasher>::const_iterator
					newPosIt = prevPosIt->second.find(destination);

				if (newPosIt != prevPosIt->second.end())
				{
					// Squash function is used to ensure captures are prioritized
					historyTableValue = 1 + squash_function(newPosIt->second);
				}
			}

			moveValue = (historyTableValue > moveValue ? historyTableValue : moveValue);

			moveSorter.insert({ moveValue, std::pair<Position, Position>(prevPos, destination) });
		}
	}

	for (std::pair<const double, std::pair<Position, Position>>& movePair : moveSorter)
	{
		result.push_back(movePair.second);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  int Agent::getMaxValue(const ChessState& game, const int depthLimit)
///
/// \brief:  - Assigns value to game state
///          - Used to determine best move for ally player
///
/// \param [in]:  game : game state being evaluated
/// \param [in]:  depthLimit : determines how many moves ahead to search
///
/// \return:  int : resulting max value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Agent::getMaxValue(const ChessState& game, const int depthLimit)
{
	const std::vector<PieceNode>& allies = (m_player == WHITE ? game.m_whitePieces : game.m_blackPieces);
	const std::vector<PieceNode>& enemies = (m_player == WHITE ? game.m_blackPieces : game.m_whitePieces);
	Color enemyPlayer = ~m_player;
	int maxValue = INT_MIN;

	std::vector<PieceNode>::const_iterator enemy = enemies.begin(),
		end = enemies.end();
	bool enemyCanMove = false;

	while (enemy != end && !enemyCanMove)
	{
		enemyCanMove = !Move::getValidMoves(enemyPlayer, *enemy, game).empty();
		enemy++;
	}

	if (enemyCanMove)
	{
		std::vector<int> indeces;
		std::vector<std::vector<Position>> allyMoves;
		int alliesSize = (int)allies.size();

		// Get all valid moves for player-controlled pieces
		for (int i = 0; i < alliesSize; i++)
		{
			std::vector<Position> validMoves = Move::getValidMoves(m_player, allies[i], game);

			if (!validMoves.empty())
			{
				indeces.push_back(i);
				allyMoves.push_back(validMoves);
			}
		}

		if (allyMoves.empty())
		{
			maxValue = INT_MIN;
		}
		// if player can make valid move and depth limit has been reached
		else if (depthLimit == 0)
		{
			maxValue = pieceValueHeuristic(game);
		}
		// if player can make valid move and depth limit has not been reached
		else
		{
			int indecesSize = (int)indeces.size();

			// Loops through every piece with valid moves
			for (int i = 0; i < indecesSize; i++)
			{
				// Loops through every valid move for current piece
				for (const Position& destination : allyMoves[i])
				{
					ChessState gameCopy(game);
					std::vector<PieceNode>::iterator pieceCopy = (m_player == WHITE ? gameCopy.m_whitePieces.begin() : gameCopy.m_blackPieces.begin()),
						end = (m_player == WHITE ? gameCopy.m_whitePieces.end() : gameCopy.m_blackPieces.end());

					// Find piece in gameCopy                                                          
					while (pieceCopy != end && pieceCopy->m_position != allies[indeces[i]].m_position)
					{
						pieceCopy++;
					}

					Move::makeMove(m_player, *pieceCopy, destination, gameCopy);
					int minValue = getMinValue(gameCopy, depthLimit - 1);

					if (minValue > maxValue)
					{
						maxValue = minValue;
					}
				}
			}
		}
	}
	else // if enemy cannot move
	{
		maxValue = INT_MAX;
	}

	return maxValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  int Agent::getMaxValue(const ChessState& game, const int depthLimit, int alpha, int beta)
///
/// \brief:  - Assign value to game state
///          - Used to determine best move for ally player
///          - Include alpha-beta pruning and quiescence search
///
/// \param [in]:  game : game state being evaluated
/// \param [in]:  depthLimit : used to determine how many moves ahead to search
/// \param [in]:  alpha : used to contain value for best choice for ally player
/// \param [in]:  beta : used to contain value for best choice for enemy player
///
/// \return:  int : resulting max value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Agent::getMaxValue(const ChessState& game, const int depthLimit, int alpha, int beta)
{
	const std::vector<PieceNode>& allies = (m_player == WHITE ? game.m_whitePieces : game.m_blackPieces);
	const std::vector<PieceNode>& enemies = (m_player == WHITE ? game.m_blackPieces : game.m_whitePieces);
	Color enemyPlayer = ~m_player;
	int maxValue = INT_MIN;

	std::vector<PieceNode>::const_iterator enemy = enemies.begin(),
		end = enemies.end();
	bool enemyCanMove = false;

	while (enemy != end && !enemyCanMove)
	{
		enemyCanMove = !Move::getValidMoves(enemyPlayer, *enemy, game).empty();
		enemy++;
	}

	if (enemyCanMove)
	{
		std::unordered_map<Position, std::vector<Position>, PositionHasher> allyMoves;

		// Get all valid moves for player-controlled pieces
		for (const PieceNode& allyPiece : allies)
		{
			std::vector<Position> validMoves = Move::getValidMoves(m_player, allyPiece, game);

			if (!validMoves.empty())
			{
				allyMoves[allyPiece.m_position] = validMoves;
			}
		}

		if (allyMoves.empty())
		{
			maxValue = INT_MIN;
		}
		// If player can make a valid move but cut off condition has been met
		else if ((depthLimit <= 0 && isQuiescent(game, allyMoves)) || depthLimit <= -MAX_QUIESCENCE_SEARCH_DEPTH)
		{
			maxValue = pieceValueHeuristic(game);
		}
		// If player can make a valid move and cut off condition has not been met
		else
		{
			std::pair<Position, Position> optimalMove; // Stores best move for current state
			std::vector<std::pair<Position, Position>> orderedMoves = orderMoves(m_player, allyMoves, game);
			bool prune = false; // Determines whether or not to keep searching
			std::vector<std::pair<Position, Position>>::const_iterator move = orderedMoves.begin(),
				movesEnd = orderedMoves.end();
			// Loops through each valid move for player
			while (move != movesEnd && !prune)
			{
				ChessState gameCopy(game);
				std::vector<PieceNode>::iterator pieceCopy = (m_player == WHITE ? gameCopy.m_whitePieces.begin() : gameCopy.m_blackPieces.begin()),
					end = (m_player == WHITE ? gameCopy.m_whitePieces.end() : gameCopy.m_blackPieces.end());

				// Find piece in gameCopy
				while (pieceCopy != end && pieceCopy->m_position != move->first)
				{
					pieceCopy++;
				}

				Move::makeMove(m_player, *pieceCopy, move->second, gameCopy);

				int minValue = getMinValue(gameCopy, depthLimit - 1, alpha, beta);

				prune = minValue >= beta;

				if (minValue > alpha)
				{
					alpha = minValue;
				}

				if (minValue > maxValue)
				{
					maxValue = minValue;
					optimalMove = *move;
				}
				move++;
			}

			// Increment optimal move in ally history table if a capture does not occur
			if (!game.m_board.posIsOccupiedByColor(optimalMove.second, enemyPlayer))
			{
				m_allyHistoryTable[optimalMove.first][optimalMove.second] += depthLimit * depthLimit;
			}
		}
	}
	else // if enemy cannot move
	{
		maxValue = INT_MAX;
	}

	return maxValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  int Agent::getMinValue(const ChessState& game, const int depthLimit)
///
/// \brief:  - Assigns value to game state
///          - Used to determine best move for enemy player
///
/// \param [in]:  game : game state being evaluated
/// \param [in]:  depthLimit : determines how many moves ahead to search
///
/// \return:  int : resulting min value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Agent::getMinValue(const ChessState& game, const int depthLimit)
{
	const std::vector<PieceNode>& allies = (m_player == WHITE ? game.m_whitePieces : game.m_blackPieces);
	const std::vector<PieceNode>& enemies = (m_player == WHITE ? game.m_blackPieces : game.m_whitePieces);
	Color enemyPlayer = ~m_player;
	int minValue = INT_MAX;

	std::vector<PieceNode>::const_iterator ally = allies.begin(),
		end = allies.end();
	bool playerCanMove = false;

	while (ally != end && !playerCanMove)
	{
		playerCanMove = !Move::getValidMoves(m_player, *ally, game).empty();
		ally++;
	}

	if (playerCanMove)
	{
		std::vector<int> indeces;
		std::vector<std::vector<Position>> enemyMoves;
		int enemiesSize = (int)enemies.size();

		// Get all valid moves for opposing player's pieces
		for (int i = 0; i < enemiesSize; i++)
		{
			std::vector<Position> validMoves = Move::getValidMoves(enemyPlayer, enemies[i], game);

			if (!validMoves.empty())
			{
				indeces.push_back(i);
				enemyMoves.push_back(validMoves);
			}
		}

		if (enemyMoves.empty())
		{
			minValue = INT_MAX;
		}
		// If opposing player can move and depth limit has been reached
		else if (depthLimit == 0)
		{
			minValue = pieceValueHeuristic(game);
		}
		// If opposing player can move and depth limit has not been reached
		else
		{
			int indecesSize = (int)indeces.size();

			// Loops through each piece with valid moves
			for (int i = 0; i < indecesSize; i++)
			{
				// Loop through each valid move for a piece
				for (const Position& destination : enemyMoves[i])
				{
					ChessState gameCopy(game);
					std::vector<PieceNode>::iterator pieceCopy = (m_player == WHITE ? gameCopy.m_blackPieces.begin() : gameCopy.m_whitePieces.begin()),
						end = (m_player == WHITE ? gameCopy.m_blackPieces.end() : gameCopy.m_whitePieces.end());

					// Find piece in gameCopy
					while (pieceCopy != end && pieceCopy->m_position != enemies[indeces[i]].m_position)
					{
						pieceCopy++;
					}

					Move::makeMove(enemyPlayer, *pieceCopy, destination, gameCopy);

					int maxValue = getMaxValue(gameCopy, depthLimit - 1);

					if (maxValue < minValue)
					{
						minValue = maxValue;
					}
				}
			}
		}
	}
	else // if player cannot move
	{
		minValue = INT_MIN;
	}

	return minValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  int Agent::getMinValue(const ChessState& game, const int depthLimit, int alpha, int beta)
///
/// \brief:  - Assign value to game state
///          - Used to determine best move for enemy player
///          - Includes alpha-beta pruning and quiescence search
///
/// \param [in]:  game : game state being evaluated
/// \param [in]:  depthLimit : determines how many moves ahead to search
/// \param [in]:  alpha : used to contain value for best choice for ally player
/// \param [in]:  beta : used to contain value for best choice for enemy player
///
/// \return:  int : resulting min value
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Agent::getMinValue(const ChessState& game, const int depthLimit, int alpha, int beta)
{
	const std::vector<PieceNode>& allies = (m_player == WHITE ? game.m_whitePieces : game.m_blackPieces);
	const std::vector<PieceNode>& enemies = (m_player == WHITE ? game.m_blackPieces : game.m_whitePieces);
	Color enemyPlayer = ~m_player;
	int minValue = INT_MAX;

	std::vector<PieceNode>::const_iterator ally = allies.begin(),
		end = allies.end();
	bool playerCanMove = false;
	while (ally != end && !playerCanMove)
	{
		playerCanMove = !Move::getValidMoves(m_player, *ally, game).empty();
		ally++;
	}

	if (playerCanMove)
	{
		std::unordered_map<Position, std::vector<Position>, PositionHasher> enemyMoves;

		for (const PieceNode& enemyPiece : enemies)
		{
			std::vector<Position> validMoves = Move::getValidMoves(enemyPlayer, enemyPiece, game);

			if (!validMoves.empty())
			{
				enemyMoves[enemyPiece.m_position] = validMoves;
			}
		}

		if (enemyMoves.empty())
		{
			minValue = INT_MAX;
		}
		// If opposing player can move and depth limit has been reached
		else if ((depthLimit <= 0 && isQuiescent(game, enemyMoves)) || depthLimit <= -MAX_QUIESCENCE_SEARCH_DEPTH)
		{
			minValue = pieceValueHeuristic(game);
		}
		// If opposing player can move and depth limit has not been reached
		else
		{
			std::pair<Position, Position> optimalMove; // Stores best move for current state
			std::vector<std::pair<Position, Position>> orderedMoves = orderMoves(enemyPlayer, enemyMoves, game);
			bool prune = false; // Determines whether or not to keep searching
			std::vector<std::pair<Position, Position>>::const_iterator move = orderedMoves.begin(),
				movesEnd = orderedMoves.end();

			while (move != movesEnd && !prune)
			{
				ChessState gameCopy(game);
				std::vector<PieceNode>::iterator pieceCopy = (enemyPlayer == WHITE ? gameCopy.m_whitePieces.begin() : gameCopy.m_blackPieces.begin()),
					end = (enemyPlayer == WHITE ? gameCopy.m_whitePieces.end() : gameCopy.m_blackPieces.end());

				// Find piece in game copy
				while (pieceCopy != end && pieceCopy->m_position != move->first)
				{
					pieceCopy++;
				}

				Move::makeMove(enemyPlayer, *pieceCopy, move->second, gameCopy);

				int maxValue = getMaxValue(gameCopy, depthLimit - 1, alpha, beta);

				prune = maxValue <= alpha;

				if (maxValue < beta)
				{
					beta = maxValue;
				}

				if (maxValue < minValue)
				{
					minValue = maxValue;
					optimalMove = *move;
				}
				move++;
			}
			// Increment optimal move in enemy history table if move is not a capture
			if (!game.m_board.posIsOccupiedByColor(optimalMove.second, m_player))
			{
				m_enemyHistoryTable[optimalMove.first][optimalMove.second] += depthLimit * depthLimit;
			}
		}
	}
	else // if player cannot move
	{
		minValue = INT_MIN;
	}

	return minValue;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string Agent::depthLimitedMinimax(PieceNode& pieceToMove, Position& pieceDestination, const int depthLimit)
///
/// \brief:  Algorithm used to determine best move for player
///
/// \param [out]:  pieceToMove : contains piece to be moved for optimal move 
/// \param [out]:  pieceDestination : contains destination of piece to be moved for optimal move
/// \param [in]:   depthLimit : determines how many moves ahead to search
///
/// \return:  std::string : string containing beginning and end position of optimal move (<file><rank><file><rank>)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Agent::depthLimitedMinimax(PieceNode& pieceToMove, Position& pieceDestination, const int depthLimit)
{
	std::string result = "";
	std::vector<int> indeces;
	std::vector<std::vector<Position>> allyMoves;
	int alliesSize = (int)m_allies.size();

	for (int i = 0; i < alliesSize; i++)
	{
		std::vector<Position> validMoves = Move::getValidMoves(m_player, m_allies[i], m_game);

		if (!validMoves.empty())
		{
			indeces.push_back(i);
			allyMoves.push_back(validMoves);
		}
	}

	// If no valid moves are possible, opposing player wins
	if (allyMoves.empty())
	{
		m_game.m_winner = ~m_player;
	}
	else // if player can move
	{
		int maxValue = INT_MIN;
		int indecesSize = (int)indeces.size();

		// Loop through each piece with valid moves
		for (int i = 0; i < indecesSize; i++)
		{
			for (Position& newPos : allyMoves[i])
			{
				ChessState gameCopy(m_game);
				std::vector<PieceNode>::iterator pieceCopy = (m_player == WHITE ? gameCopy.m_whitePieces.begin() : gameCopy.m_blackPieces.begin()),
					end = (m_player == WHITE ? gameCopy.m_whitePieces.end() : gameCopy.m_blackPieces.end());

				// Find piece in gameCopy
				while (pieceCopy != end && pieceCopy->m_position != m_allies[indeces[i]].m_position)
				{
					pieceCopy++;
				}

				Move::makeMove(m_player, *pieceCopy, newPos, gameCopy);

				int tmp = getMaxValue(gameCopy, depthLimit);

				if (tmp > maxValue)
				{
					maxValue = tmp;
					pieceToMove = m_allies[indeces[i]];
					pieceDestination = newPos;
				}
			}
		}

		std::string promotion = "";

		if (pieceToMove.m_pieceType == PAWN)
		{
			// If pawn is to be promoted
			if (pieceDestination.m_y == NUM_RANKS - 1 || pieceDestination.m_y == 0)
			{
				// Make promotion "q"; Will always promote to queen
				promotion = "q";
			}
		}

		result += toFileAndRank(pieceToMove.m_position) + toFileAndRank(pieceDestination) + promotion;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string Agent::prunedDepthLimitedMinimax(PieceNode& pieceToMove, Position& pieceDestination, const int depthLimit)
///
/// \brief:  - Algorithm used to determine next move for player
///          - Includes alpha-beta pruning and quiescent search
///
/// \param [out]:  pieceToMove : contains piece to be moved for optimal move 
/// \param [out]:  pieceDestination : contains destination of piece to be moved for optimal move
/// \param [in]:   depthLimit : determines how many moves ahead to search
///
/// \return:  std::string : string containing beginning and end position of optimal move (<file><rank><file><rank>)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MoveNode Agent::prunedDepthLimitedMinimax(PieceNode& pieceToMove, Position& pieceDestination, const int depthLimit)
{
	MoveNode result;
	std::unordered_map<PieceNode*, std::vector<Position>> allyMoves;

	for (PieceNode& allyPiece : m_allies)
	{
		std::vector<Position> validMoves = Move::getValidMoves(m_player, allyPiece, m_game);

		if (!validMoves.empty())
		{
			allyMoves[&allyPiece] = validMoves;
		}
	}

	// If no valid moves are possible, opposing player wins
	if (allyMoves.empty())
	{
		m_game.m_winner = ~m_player;
	}
	else // if player can move
	{
		int maxValue = INT_MIN;

		// Loop through each ally piece with valid moves
		for (const std::pair<PieceNode*, std::vector<Position>>& pieceMoves : allyMoves)
		{
			for (const Position& destination : pieceMoves.second)
			{
				ChessState gameCopy(m_game);
				std::vector<PieceNode>::iterator pieceCopy = (m_player == WHITE ? gameCopy.m_whitePieces.begin() : gameCopy.m_blackPieces.begin()),
					end = (m_player == WHITE ? gameCopy.m_whitePieces.end() : gameCopy.m_blackPieces.end());

				// Find piece in gameCopy
				while (pieceCopy != end && pieceCopy->m_position != (pieceMoves.first)->m_position)
				{
					pieceCopy++;
				}

				Move::makeMove(m_player, *pieceCopy, destination, gameCopy);

				int tmp = getMaxValue(gameCopy, depthLimit, INT_MIN, INT_MAX);

				if (tmp > maxValue)
				{
					maxValue = tmp;
					pieceToMove = *(pieceMoves.first);
					pieceDestination = destination;
				}
			}
		}

		PieceType promotion = PieceType::NONE;

		if (pieceToMove.m_pieceType == PAWN)
		{
			// If pawn is to be promoted
			if (pieceDestination.m_y == NUM_RANKS - 1 || pieceDestination.m_y == 0)
			{
				// Make promotion "q"; Will always promote to queen
				promotion = PieceType::QUEEN;
			}
		}

		// Set result string
		result.m_source = pieceToMove.m_position;
		result.m_destination = pieceDestination;
		result.m_promotion = promotion;
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  Agent::Agent(const Color player, ChessState& game)
///
/// \brief:  Creates an Agent object
///
/// \param [in]:  player : player color assigned to agent
/// \param [in]:  game : game to be played by agent
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Agent::Agent(const Color player, ChessState& game) :
	m_player(player),
	m_game(game),
	m_allies((m_player == WHITE ? game.m_whitePieces : game.m_blackPieces)),
	m_enemies((m_player == WHITE ? game.m_blackPieces : game.m_whitePieces))
{
	srand(time(NULL));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  Agent::~Agent()
///
/// \brief:  Destroy an Agent object
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Agent::~Agent()
{
	clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  Agent::clear()
///
/// \brief:  Clear calling object
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Agent::clear()
{
	m_player = NEUTRAL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string Agent::makeRandomMove()
///
/// \brief:  Chess-playing algorithms that makes random valid moves
///
/// \return:  std::string : contains random move (<file><rank><file><rank>)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Agent::makeRandomMove()
{
	std::string result = "";
	std::vector<int> indeces;
	std::vector<std::vector<Position>> validMoves;
	int alliesSize = (int)m_allies.size();

	for (int i = 0; i < alliesSize; i++)
	{
		std::vector<Position> moves = Move::getValidMoves(m_player, m_allies[i], m_game);

		if (!moves.empty())
		{
			indeces.push_back(i);
			validMoves.push_back(moves);
		}
	}

	if (validMoves.empty())
	{
		m_game.m_winner = ~m_player;
	}
	else // if player can move
	{
		// Pick random piece with valid moves
		int permIndex = rand() % (int)indeces.size();
		PieceNode* randPiece = &m_allies[indeces[permIndex]];
		result += toFileAndRank(randPiece->m_position);

		// Pick random move from piece's valid moves
		int moveIndex = rand() % (int)validMoves[permIndex].size();
		Position randMove = validMoves[permIndex][moveIndex];

		PieceType promotion = NONE;

		if (randPiece->m_pieceType == PAWN)
		{
			// If pawn is to be promoted
			if (randMove.m_y == NUM_RANKS - 1 || randMove.m_y == 0)
			{
				// Select a random promotion (Knight, Bishop, Rook, Queen)
				promotion = static_cast<PieceType>((rand() % 4) + 2);
			}
		}

		Move::makeMove(m_player, *randPiece, randMove, m_game, promotion);
		result += toFileAndRank(randMove);

		if (promotion != NONE)
		{
			switch (promotion)
			{
			case KNIGHT:
				result += 'n';
				break;
			case BISHOP:
				result += 'b';
				break;
			case ROOK:
				result += 'r';
				break;
			case QUEEN:
				result += 'q';
				break;
			default:
				break;
			}
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string Agent::iterDepthLimitedMinimax()
///
/// \brief:  Chess-playing algorithm that attempts to find optimal move
///
/// \return:  std::string : contains random move (<file><rank><file><rank>)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string Agent::iterDepthLimitedMinimax()
{
	std::string result = "";
	PieceNode pieceToMove;
	Position destination;

	for (int i = 0; i < MAX_DEPTH; i++)
	{
		result = depthLimitedMinimax(pieceToMove, destination, i);
	}

	std::vector<PieceNode>::iterator piece = m_allies.begin(),
		end = m_allies.end();


	while (piece != end && piece->m_position != pieceToMove.m_position)
	{
		piece++;
	}

	Move::makeMove(m_player, *piece, destination, m_game);

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string Agent::prunedIterDepthLimitedMinimax()
///
/// \brief:  - Chess-playing algorithms that attempts to find optimal move
///          - Includes alpha-beta pruning and quiescent search
///
/// \return:  std::string : contains random move (<file><rank><file><rank>)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MoveNode Agent::prunedIterDepthLimitedMinimax()
{
	MoveNode result;
	PieceNode pieceToMove;
	Position destination;

	for (int i = 0; i < MAX_DEPTH; i++)
	{
		result = prunedDepthLimitedMinimax(pieceToMove, destination, i);
	}

	std::vector<PieceNode>::iterator piece = m_allies.begin(),
		end = m_allies.end();

	// Find piece to move in player-controlled pieces
	while (piece != end && piece->m_position != pieceToMove.m_position)
	{
		piece++;
	}


	Move::makeMove(m_player, *piece, destination, m_game);

	return result;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn:  std::string Agent::iterTimeLimitedMinimax(const double timeRemaining)
///
/// \brief:  - Chess-playing algorithm that attempts to find optimal move
///          - Search time dependent of time heuristic
///          - Includes alpha-beta pruning and quiescent search
///
/// \param [in]:  timeRemaining : time remaining until player times out
///
/// \return:  std::string : contains random move (<file><rank><file><rank>)
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MoveNode Agent::iterTimeLimitedMinimax(const double timeRemaining)
{
	MoveNode result;
	PieceNode pieceToMove;
	Position destination;

	const chronoTime startTime = std::chrono::high_resolution_clock::now();
	double timeElapsed = 0.0;
	int depth = 0;

	do
	{
		result = prunedDepthLimitedMinimax(pieceToMove, destination, depth);
		timeElapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - startTime).count();
		depth += 1;
	} while (timeHeuristic(timeElapsed, timeRemaining));

	std::vector<PieceNode>::iterator piece = m_allies.begin(),
		end = m_allies.end();

	// Find piece to move in player-controlled pieces
	while (piece != end && piece->m_position != pieceToMove.m_position)
	{
		piece++;
	}

	Move::makeMove(m_player, *piece, destination, m_game);

	return result;
}