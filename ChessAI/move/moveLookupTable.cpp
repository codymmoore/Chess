#include "moveLookupTable.h"

#include <vector>
#include <bit>
#include <math.h>
#include <random>
#include <stack>

#include "../util/bitboard/bitboardUtil.h"

using util::bitboard::shiftBitboard;
using util::bitboard::Shift;
using util::bitboard::up;
using util::bitboard::left;
using util::bitboard::down;
using util::bitboard::right;

namespace move
{
	const int SQUARE_COUNT = FILE_COUNT * RANK_COUNT;

	Bitboard knightMoveLookupTable[SQUARE_COUNT];

	Bitboard bishopBlockerMask[SQUARE_COUNT];
	Bitboard bishopBlockerHashCoefficients[SQUARE_COUNT];
	std::vector<Bitboard> bishopMoveLookupTable[SQUARE_COUNT];

	Bitboard rookBlockerMask[SQUARE_COUNT];
	Bitboard rookBlockerHashCoefficients[SQUARE_COUNT];
	std::vector<Bitboard> rookMoveLookupTable[SQUARE_COUNT];

	Bitboard kingMoveLookupTable[SQUARE_COUNT];

	Bitboard getKnightMoveBoard(const int positionIndex)
	{
		return knightMoveLookupTable[positionIndex];
	}

	Bitboard getBishopMoveBoard(const int positionIndex, const Bitboard occupancyBoard)
	{
		const Bitboard blockerMask = bishopBlockerMask[positionIndex];
		const int blockerCount = std::popcount(blockerMask);
		const Bitboard blockerBoard = blockerMask & occupancyBoard;
		const std::vector<Bitboard>& moveBoards = bishopMoveLookupTable[positionIndex];
		const int blockerHash = (bishopBlockerHashCoefficients[positionIndex] * blockerBoard) >> (64 - blockerCount);

		return moveBoards[blockerHash];
	}

	Bitboard getRookMoveBoard(const int positionIndex, const Bitboard occupancyBoard)
	{
		const Bitboard blockerMask = rookBlockerMask[positionIndex];
		const int blockerCount = std::popcount(blockerMask);
		const Bitboard blockerBoard = blockerMask & occupancyBoard;
		const std::vector<Bitboard>& moveBoards = rookMoveLookupTable[positionIndex];
		const int blockerHash = (rookBlockerHashCoefficients[positionIndex] * blockerBoard) >> (64 - blockerCount);

		return moveBoards[blockerHash];
	}

	Bitboard getKingMoveBoard(const int positionIndex)
	{
		return kingMoveLookupTable[positionIndex];
	}

	void populateKnightMoveLookupTable()
	{
		static std::vector<Shift> knightShifts = {
			up(2) + left(1),
			up(1) + left(2),
			up(2) + right(1),
			up(1) + right(2),
			down(2) + left(1),
			down(1) + left(2),
			down(2) + right(1),
			down(1) + right(2)
		};

		for (int i = 0; i < FILE_COUNT * RANK_COUNT; i++)
		{
			const Bitboard source = Bitboard(1) << i;
			knightMoveLookupTable[i] = 0;

			for (const Shift& shift : knightShifts)
			{
				knightMoveLookupTable[i] |= shiftBitboard(source, shift);
			}
		}
	}

	Bitboard generateBlockerMask(const int positionIndex, const std::vector<Shift>& shifts)
	{
		const int x = positionIndex % FILE_COUNT;
		const int y = positionIndex / FILE_COUNT;
		const Bitboard source = Bitboard(1) << positionIndex;
		Bitboard blockerMask = 0;

		for (const Shift& shift : shifts)
		{
			Shift shiftIt = shift;
			while ((shiftIt.horizontal == 0 || (x + shiftIt.horizontal > 0 && x + shiftIt.horizontal < FILE_COUNT - 1))
				&& (shiftIt.vertical == 0 || (y + shiftIt.vertical > 0 && y + shiftIt.vertical < RANK_COUNT - 1)))
			{
				blockerMask |= shiftBitboard(source, shiftIt);
				shiftIt += shift;
			}
		}

		return blockerMask;
	}

	std::vector<Bitboard> generateBlockerBoards(const int positionIndex, Bitboard blockerMask)
	{
		std::vector<int> blockerPositions;

		while (blockerMask)
		{
			blockerPositions.push_back(util::bitboard::popLsb(blockerMask));
		}

		const int blockerCount = blockerPositions.size();
		const int variationCount = exp2(blockerCount);
		std::vector<Bitboard> blockerBoards(variationCount);

		for (int i = 0; i < variationCount; i++)
		{
			Bitboard blockerBoard = 0;
			for (int j = 0; j < blockerCount; j++)
			{
				if (i & (1 << j)) // if bit j is set in blocker variation i
				{
					blockerBoard |= (Bitboard(1) << blockerPositions[j]);
				}
			}
			blockerBoards[i] = blockerBoard;
		}

		return blockerBoards;
	}

	Bitboard generateMoves(const int positionIndex, const Bitboard blockerBoard, const std::vector<Shift>& shifts)
	{
		const int x = positionIndex % FILE_COUNT;
		const int y = positionIndex / FILE_COUNT;
		const Bitboard source = Bitboard(1) << positionIndex;
		Bitboard moveBoard = 0;

		for (const Shift& shift : shifts)
		{
			Shift shiftIt = shift;
			while (
				x + shiftIt.horizontal >= 0 && x + shiftIt.horizontal < FILE_COUNT
				&& y + shiftIt.vertical >= 0 && y + shiftIt.vertical < RANK_COUNT)
			{
				Bitboard destination = shiftBitboard(source, shiftIt);
				moveBoard |= destination;
				if (destination & blockerBoard)
				{
					break;
				}
				shiftIt += shift;
			}
		}

		return moveBoard;
	}

	Bitboard getRandomBitboard()
	{
		static std::mt19937_64 rng(std::random_device{}());
		static std::uniform_int_distribution<Bitboard> dist(0, ~0ULL);
		return dist(rng);
	}

	Bitboard generateRandomCoefficient()
	{
		return getRandomBitboard() & getRandomBitboard() & getRandomBitboard();
	}

	Bitboard findBlockerHashCoefficient(const Bitboard blockerMask, const std::vector<Bitboard>& blockerBoards, std::vector<Bitboard>& moveBoards)
	{
		static const int MIN_POPCOUNT_THRESHOLD = 6;
		const int blockerCount = std::popcount(blockerMask);

		while (true)
		{
			const Bitboard coefficient = generateRandomCoefficient();
			if (std::popcount((blockerMask * coefficient) & 0xFF00000000000000) < MIN_POPCOUNT_THRESHOLD)
			{
				continue;
			}

			std::vector<Bitboard> moveTable(moveBoards.size(), 0);
			bool isMagic = true;

			for (int i = 0; i < moveBoards.size(); i++)
			{
				const Bitboard blockerBoard = blockerBoards[i];
				const Bitboard moveBoard = moveBoards[i];
				const int blockerHash = (blockerBoard * coefficient) >> (64 - blockerCount);

				if (moveTable[blockerHash] == 0)
				{
					moveTable[blockerHash] = moveBoard;
				}
				else if (moveTable[blockerHash] != moveBoard)
				{
					isMagic = false;
					break;
				}
			}

			if (isMagic)
			{
				moveBoards = moveTable;
				return coefficient;
			}
		}
	}

	void populateBishopMoveLookupTable()
	{
		static const std::vector<Shift> bishopShifts = {
			up(1) + left(1),
			up(1) + right(1),
			down(1) + left(1),
			down(1) + right(1)
		};

		for (int positionIndex = 0; positionIndex < SQUARE_COUNT; positionIndex++)
		{
			const Bitboard blockerMask = generateBlockerMask(positionIndex, bishopShifts);
			const std::vector<Bitboard> blockerBoards = generateBlockerBoards(positionIndex, blockerMask);
			std::vector<Bitboard> moves;

			bishopBlockerMask[positionIndex] = blockerMask;

			for (const Bitboard blockerBoard : blockerBoards)
			{
				moves.push_back(generateMoves(positionIndex, blockerBoard, bishopShifts));
			}

			bishopMoveLookupTable[positionIndex].resize(blockerBoards.size());

			const Bitboard blockerHashCoefficient = findBlockerHashCoefficient(blockerMask, blockerBoards, moves);
			const int blockerCount = std::popcount(blockerMask);
			bishopBlockerHashCoefficients[positionIndex] = blockerHashCoefficient;

			for (const Bitboard blockerBoard : blockerBoards)
			{
				const int blockerHash = (blockerBoard * blockerHashCoefficient) >> (64 - blockerCount);
				bishopMoveLookupTable[positionIndex] = moves;
			}
		}
	}

	void populateRookMoveLookupTable()
	{
		static const std::vector<Shift> rookShifts = {
			up(1),
			down(1),
			left(1),
			right(1)
		};

		for (int positionIndex = 0; positionIndex < SQUARE_COUNT; positionIndex++)
		{
			const Bitboard blockerMask = generateBlockerMask(positionIndex, rookShifts);
			const std::vector<Bitboard> blockerBoards = generateBlockerBoards(positionIndex, blockerMask);
			std::vector<Bitboard> moves;

			rookBlockerMask[positionIndex] = blockerMask;

			for (const Bitboard blockerBoard : blockerBoards)
			{
				moves.push_back(generateMoves(positionIndex, blockerBoard, rookShifts));
			}

			rookMoveLookupTable[positionIndex].resize(blockerBoards.size());

			const Bitboard blockerHashCoefficient = findBlockerHashCoefficient(blockerMask, blockerBoards, moves);
			const int blockerCount = std::popcount(blockerMask);
			rookBlockerHashCoefficients[positionIndex] = blockerHashCoefficient;

			for (const Bitboard blockerBoard : blockerBoards)
			{
				const int blockerHash = (blockerBoard * blockerHashCoefficient) >> (64 - blockerCount);
				rookMoveLookupTable[positionIndex] = moves;
			}
		}
	}

	void populateKingMoveLookupTable()
	{
		static std::vector<Shift> kingShifts = {
			up(1),
			down(1),
			left(1),
			right(1),
			up(1) + left(1),
			up(1) + right(1),
			down(1) + left(1),
			down(1) + right(1)
		};

		for (int i = 0; i < FILE_COUNT * RANK_COUNT; i++)
		{
			const Bitboard source = Bitboard(1) << i;
			kingMoveLookupTable[i] = 0;

			for (const Shift& shift : kingShifts)
			{
				kingMoveLookupTable[i] |= shiftBitboard(source, shift);
			}
		}
	}

	struct LookupTableInitializer
	{
		LookupTableInitializer()
		{
			populateKnightMoveLookupTable();
			populateBishopMoveLookupTable();
			populateRookMoveLookupTable();
			populateKingMoveLookupTable();
		}
	} lookupTableInitializer;
}
