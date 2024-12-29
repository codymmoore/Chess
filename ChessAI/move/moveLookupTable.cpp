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

	Bitboard knightLookupTable[SQUARE_COUNT];

	Bitboard bishopBlockerMask[SQUARE_COUNT];
	Bitboard bishopBlockerHashCoefficients[SQUARE_COUNT];
	std::vector<Bitboard> bishopLookupTable[SQUARE_COUNT];

	Bitboard getKnightMoveBoard(const int positionIndex)
	{
		return knightLookupTable[positionIndex];
	}

	Bitboard getBishopMoveBoard(const int positionIndex, const Bitboard occupancyBoard)
	{
		const Bitboard blockerMask = bishopBlockerMask[positionIndex];
		const int blockerCount = std::popcount(blockerMask);
		const Bitboard blockerBoard = blockerMask & occupancyBoard;
		const std::vector<Bitboard>& moveBoards = bishopLookupTable[positionIndex];
		const int blockerHash = (bishopBlockerHashCoefficients[positionIndex] * blockerBoard) >> (64 - blockerCount);

		return moveBoards[blockerHash];
	}

	void populateKnightLookupTable()
	{
		static std::vector<Shift> knightShifts = {
			Shift({ up(2), left(1) }),
			Shift({ up(1), left(2) }),
			Shift({ up(2), right(1) }),
			Shift({ up(1), right(2) }),
			Shift({ down(2), left(1) }),
			Shift({ down(1), left(2) }),
			Shift({ down(2), right(1) }),
			Shift({ down(1), right(2) })
		};

		for (int i = 0; i < FILE_COUNT * RANK_COUNT; i++)
		{
			const Bitboard source = Bitboard(1) << i;
			knightLookupTable[i] = 0;

			for (const Shift& shift : knightShifts)
			{
				knightLookupTable[i] |= shiftBitboard(source, shift);
			}
		}
	}

	Bitboard generateBishopBlockerMask(const int positionIndex)
	{
		const int x = positionIndex % FILE_COUNT;
		const int y = positionIndex / FILE_COUNT;
		const Bitboard source = Bitboard(1) << positionIndex;
		Bitboard blockerMask = 0;

		int distance = 1;
		while (x - distance > 0 && y - distance > 0)
		{
			blockerMask |= shiftBitboard(source, { up(distance), left(distance) });
			distance += 1;
		}

		distance = 1;
		while (x + distance < FILE_COUNT - 1 && y - distance > 0)
		{
			blockerMask |= shiftBitboard(source, { up(distance), right(distance) });
			distance += 1;
		}

		distance = 1;
		while (x + distance < FILE_COUNT - 1 && y + distance < RANK_COUNT - 1)
		{
			blockerMask |= shiftBitboard(source, { down(distance), right(distance) });
			distance += 1;
		}

		distance = 1;
		while (x - distance > 0 && y + distance < RANK_COUNT - 1)
		{
			blockerMask |= shiftBitboard(source, { down(distance), left(distance) });
			distance += 1;
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
			blockerBoards.push_back(blockerBoard);
		}

		return blockerBoards;
	}

	Bitboard generateBishopMoves(const int positionIndex, const Bitboard blockerBoard)
	{
		const int x = positionIndex % FILE_COUNT;
		const int y = positionIndex / FILE_COUNT;
		const Bitboard source = Bitboard(1) << positionIndex;
		Bitboard moveBoard = 0;

		int distance = 1;
		while (x - distance >= 0 && y - distance >= 0)
		{
			Bitboard destination = shiftBitboard(source, { up(distance), left(distance) });
			moveBoard |= destination;
			if (destination & blockerBoard)
			{
				break;
			}
			distance += 1;
		}

		distance = 1;
		while (x + distance < FILE_COUNT && y - distance >= 0)
		{
			Bitboard destination = shiftBitboard(source, { up(distance), right(distance) });
			moveBoard |= destination;
			if (destination & blockerBoard)
			{
				break;
			}
			distance += 1;
		}

		distance = 1;
		while (x + distance < FILE_COUNT && y + distance < RANK_COUNT)
		{
			Bitboard destination = shiftBitboard(source, { down(distance), right(distance) });
			moveBoard |= destination;
			if (destination & blockerBoard)
			{
				break;
			}
			distance += 1;
		}

		distance = 1;
		while (x - distance >= 0 && y + distance < RANK_COUNT)
		{
			Bitboard destination = shiftBitboard(source, { down(distance), left(distance) });
			moveBoard |= destination;
			if (destination & blockerBoard)
			{
				break;
			}
			distance += 1;
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
		const Bitboard randomBitboard = getRandomBitboard();
		return randomBitboard & randomBitboard & randomBitboard;
	}

	Bitboard findBlockerHashCoefficient(const Bitboard blockerMask, const std::vector<Bitboard>& blockerBoards, std::vector<Bitboard>& moveBoards)
	{
		static const int MIN_POPCOUNT_THRESHOLD = 6;
		const int blockerCount = std::popcount(blockerMask);
		std::vector<Bitboard> moveTable(moveBoards.size());

		while (true)
		{
			const Bitboard coefficient = generateRandomCoefficient();
			if (std::popcount((blockerMask * coefficient) >> 64) < MIN_POPCOUNT_THRESHOLD)
			{
				continue;
			}

			std::fill(moveTable.begin(), moveTable.end(), 0);
			bool isMagic = true;

			for (int i = 0; i < blockerCount; i++)
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

	void populateBishopLookupTable()
	{
		for (int positionIndex = 0; positionIndex < SQUARE_COUNT; positionIndex++)
		{
			const Bitboard blockerMask = generateBishopBlockerMask(positionIndex);
			const std::vector<Bitboard> blockerBoards = generateBlockerBoards(positionIndex, blockerMask);
			std::vector<Bitboard> moves;

			bishopBlockerMask[positionIndex] = blockerMask;

			for (const Bitboard blockerBoard : blockerBoards)
			{
				moves.push_back(generateBishopMoves(positionIndex, blockerBoard));
			}

			bishopLookupTable[positionIndex].resize(blockerBoards.size());

			const Bitboard blockerHashCoefficient = findBlockerHashCoefficient(blockerMask, blockerBoards, moves);
			const int blockerCount = std::popcount(blockerMask);
			bishopBlockerHashCoefficients[positionIndex] = blockerHashCoefficient;

			for (const Bitboard blockerBoard : blockerBoards)
			{
				const int blockerHash = (blockerBoard * blockerHashCoefficient) >> (64 - blockerCount);
				bishopLookupTable[positionIndex] = moves;
			}
		}
	}

	struct LookupTableInitializer
	{
		LookupTableInitializer()
		{
			populateKnightLookupTable();
			populateBishopLookupTable();
		}
	} lookupTableInitializer;
}
