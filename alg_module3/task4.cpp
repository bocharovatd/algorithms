// Общие требования для всех задач
// Ввод/вывод отделены от решения.
// Не должно быть утечек памяти.
// Задача 4. Пятнашки неточные
// Написать алгоритм для решения игры в “пятнашки”. Решением задачи является приведение к виду:
// [ 1 2 3 4 ] [ 5 6 7 8 ] [ 9 10 11 12] [ 13 14 15 0 ], где 0 задает пустую ячейку.
// Достаточно найти хотя бы какое-то решение.
// Число перемещений костяшек не обязано быть минимальным.
// Формат ввода
// Начальная расстановка.
// Формат вывода
// Если вам удалось найти решение, то в первой строке файла выведите число перемещений,
// которое требуется сделать в вашем решении.
// А во второй строке выведите соответствующую последовательность ходов:
// L означает, что в результате перемещения костяшка сдвинулась влево, R – вправо, U – вверх, D – вниз.
// Если же выигрышная конфигурация недостижима, то выведите в выходной файл одно число −1.

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

const int SideSize = 4;
const int FieldSize = SideSize * SideSize;
const std::array<char, FieldSize> FinishField({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0});

class GameState {
public:
    GameState(const std::array<char, FieldSize> &_field);

    bool CanMoveLeft() const;
    bool CanMoveUp() const;
    bool CanMoveRight() const;
    bool CanMoveDown() const;

    GameState MoveLeft() const;
    GameState MoveUp() const;
    GameState MoveRight() const;
    GameState MoveDown() const;

    bool IsFinish() const;

    char GetFieldValue(size_t pos) const { return field[pos]; }
    char GetNullPosition() const;
    std::vector<GameState> GetNextStates(const GameState& state);

    bool operator==(const GameState &state) const { return field == state.field; }
    bool operator!=(const GameState &state) const { return !(*this == state); }
    bool operator<(const GameState &state) const;

private:
    std::array<char, FieldSize> field;
    int zeroPos;
};

GameState::GameState(const std::array<char, FieldSize> &_field) : field(_field),
                                                                  zeroPos(-1) {
    for (int i = 0; i < field.size(); ++i) {
        if (field[i] == 0) {
            zeroPos = i;
            break;
        }
    }
    assert(zeroPos >= 0);
}

bool GameState::CanMoveLeft() const {
    return zeroPos % SideSize < SideSize - 1;
}

bool GameState::CanMoveUp() const {
    return zeroPos < FieldSize - SideSize;
}

bool GameState::CanMoveRight() const {
    return zeroPos % SideSize > 0;
}

bool GameState::CanMoveDown() const {
    return zeroPos >= SideSize;
}

GameState GameState::MoveLeft() const {
    assert(CanMoveLeft());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + 1]);
    ++newState.zeroPos;
    return newState;
}

GameState GameState::MoveUp() const {
    assert(CanMoveUp());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos + SideSize]);
    newState.zeroPos += SideSize;
    return newState;
}

GameState GameState::MoveRight() const {
    assert(CanMoveRight());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - 1]);
    --newState.zeroPos;
    return newState;
}

GameState GameState::MoveDown() const {
    assert(CanMoveDown());
    GameState newState(*this);
    std::swap(newState.field[zeroPos], newState.field[zeroPos - SideSize]);
    newState.zeroPos -= SideSize;
    return newState;
}

bool GameState::IsFinish() const {
    return field == FinishField;
}

char GameState::GetNullPosition() const {
    auto it = field.begin();
    while (it != field.end()) {
        if (*it == 0) {
            return it - field.begin();
        }
        ++it;
    }
    assert(false);
}

std::vector<std::pair<GameState, char>> GetNextStates(const GameState& state) {
    std::vector<std::pair<GameState, char>> nextStates;
    if (state.CanMoveLeft()) {
        GameState newState = state.MoveLeft();
        nextStates.push_back(std::make_pair(newState, 'L'));
    }
    if (state.CanMoveUp()) {
        GameState newState = state.MoveUp();
        nextStates.push_back(std::make_pair(newState, 'U'));
    }
    if (state.CanMoveRight()) {
        GameState newState = state.MoveRight();
        nextStates.push_back(std::make_pair(newState, 'R'));
    }
    if (state.CanMoveDown()) {
        GameState newState = state.MoveDown();
        nextStates.push_back(std::make_pair(newState, 'D'));
    }
    return nextStates;
}

bool GameState::operator<(const GameState &state) const {
    return field < state.field;
}

std::ostream &operator<<(std::ostream &output, const GameState &state) {
    for (size_t y = 0; y < SideSize; ++y) {
        for (size_t x = 0; x < SideSize; ++x) {
            output<< static_cast<int>(state.GetFieldValue(y * SideSize + x)) << " ";
        }
        output << std::endl;
    }
    output << std::endl;
    return output;
}

struct StateHasher {
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        size_t p = 1;
        for (int i = 0; i < FieldSize; ++i) {
            hash += state.GetFieldValue(i) * p;
            p *= 137;
        }
        return hash;
    }
};

int heuristics(const GameState &state) {
    int result = 0;
    for (char pos = 0; pos < 16; ++pos) {
        char valueOnPos = state.GetFieldValue(pos);
        result += std::fabs((valueOnPos + 15) % 16 / 4 - pos / 4) + std::fabs((valueOnPos + 15) % 16 % 4 - pos % 4);
    }
    return result;
}

bool Has15thSolution(const GameState &state) {
    int countInversions = 0;
    for (char i = 0; i < 16; ++i) {
        for (char j = i + 1; j < 16; ++j) {
            char valueA = state.GetFieldValue(i);
            char valueB = state.GetFieldValue(j);
            if (valueA != 0 && valueB != 0) {
                if (valueA > valueB) {
                    ++countInversions;
                }
            }
        }
    }
    return ((countInversions + (4 - state.GetNullPosition() / 4)) % 2 != 0);
}

std::string Get15thSolution(const GameState &state) {
    std::unordered_map<GameState, int, StateHasher> dist;
    dist[state] = 0;
    std::set<std::pair<int, GameState>> priorityQueue;
	priorityQueue.insert(std::make_pair(dist[state] + heuristics(state), state));
    std::unordered_map<GameState, char, StateHasher> visited;
    visited[state] = 'S';
    while (!priorityQueue.empty()) {
        GameState currentState = priorityQueue.begin()->second;
		priorityQueue.erase(priorityQueue.begin());
        if (currentState.IsFinish()) {
            break;
        }
        std::vector<std::pair<GameState, char>> nextStatesList = GetNextStates(currentState);
        for (int i = 0; i < nextStatesList.size(); ++i) {
            GameState newState = nextStatesList[i].first;
            char move = nextStatesList[i].second;
            if (visited.find(newState) == visited.end()) {
                int heuristicsNewState = heuristics(newState);
                if ((priorityQueue.size() < 1000 || heuristicsNewState < 40) 
                && (priorityQueue.size() < 100000 || heuristicsNewState < 27)
                && (priorityQueue.size() < 200000 || heuristicsNewState < 20)
                && (priorityQueue.size() < 400000 || heuristicsNewState < 15)
                && (priorityQueue.size() < 500000 || heuristicsNewState < 12)) {
                    dist[newState] = dist[currentState] + 1;
                    priorityQueue.insert(std::make_pair(dist[newState] + heuristics(newState), newState));
                    visited[newState] = move;
                }
            }
            else if (dist[newState] > dist[currentState] + 1) {
                int heuristicsNewState = heuristics(newState);
                priorityQueue.erase(std::make_pair(dist[newState] + heuristicsNewState, newState));
                dist[newState] = dist[currentState] + 1;
                priorityQueue.insert(std::make_pair(dist[newState] + heuristicsNewState, newState));
                visited[newState] = move;
            }
		}
    }
    std::string result;
    GameState current(FinishField);
    char move = visited[current];
    while (move != 'S') {
        result += move;
        switch (move) {
        case 'L':
            current = current.MoveRight();
            break;
        case 'U':
            current = current.MoveDown();
            break;
        case 'R':
            current = current.MoveLeft();
            break;
        case 'D':
            current = current.MoveUp();
            break;
        default:
            assert(false);
            break;
        }
        move = visited[current];
    }

    std::reverse(result.begin(), result.end());
    return result;
}

void Run(std::istream &input, std::ostream &output) {
    std::array<char, 16> field;
    auto it = field.begin();
    while (it != field.end()) {
        int num = 0;
        input >> num;
        *it = num;
        ++it;
    }
    GameState state(field);
    if (!Has15thSolution(state)) {
        output << -1 << std::endl;
        return;
    }
    std::string result = Get15thSolution(state);
    output << result.length() << std::endl;
    output << result << std::endl;
}

void Test15thSolution() {
        {
            std::stringstream input;
            std::stringstream output;
            input << "1 2 3 0 5 6 7 4 9 10 11 8 13 14 15 12 ";
            Run(input, output);
            assert(output.str() == "3\nUUU\n");
        }
        {
            std::stringstream input;
            std::stringstream output;
            input << "1 2 0 3 5 6 7 4 9 10 11 8 13 14 15 12 ";
            Run(input, output);
            assert(output.str() == "4\nLUUU\n");
        }
}

int main() {
    //Run(std::cin, std::cout);
    Test15thSolution();
}