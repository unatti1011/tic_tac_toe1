#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

class Player {
public:
    char letter;
    Player(char l) : letter(l) {}
    virtual int get_move(vector<char> board) = 0; // Pure virtual function
};

class HumanPlayer : public Player {
public:
    HumanPlayer(char l) : Player(l) {}

    int get_move(vector<char> board) override {
        int square;
        bool valid_move = false;
        while (!valid_move) {
            cout << letter << "'s turn. Input move (0-8): ";
            cin >> square;
            if (cin.fail() || square < 0 || square > 8 || board[square] != ' ') {
                cout << "Invalid square. Try again." <<endl;
                cin.clear();
                cin.ignore(10000, '\n');
            } else {
                valid_move = true;
            }
        }
        return square;
    }
};

class RandomComputerPlayer : public Player {
public:
    RandomComputerPlayer(char l) : Player(l) {}

    int get_move(vector<char> board) override {
        srand(time(nullptr));
        vector<int> available_moves;
        for (int i = 0; i < board.size(); ++i) {
            if (board[i] == ' ') {
                available_moves.push_back(i);
            }
        }
        return available_moves[rand() % available_moves.size()];
    }
};

class SmartComputerPlayer : public Player {
public:
    SmartComputerPlayer(char l) : Player(l) {}

    int get_move(vector<char> board) override {
        if (available_moves(board).size() == 9) {
            srand(time(nullptr));
            return rand() % 9;
        } else {
            return minimax(board, letter).first;
        }
    }

private:
    pair<int, int> minimax(vector<char> board, char player) {
        char opponent = (player == 'X') ? 'O' : 'X';
        if (check_winner(board, opponent)) {
            return {-1, -10}; // Losing score
        }
        if (available_moves(board).empty()) {
            return {-1, 0}; // Tie
        }

        vector<int> moves = available_moves(board);
        pair<int, int> best_move = {-1, (player == letter) ? -1000 : 1000};

        for (int move : moves) {
            board[move] = player;
            pair<int, int> score = minimax(board, opponent);
            board[move] = ' ';
            score.first = move;
            if (player == letter) {
                if (score.second > best_move.second) {
                    best_move = score;
                }
            } else {
                if (score.second < best_move.second) {
                    best_move = score;
                }
            }
        }
        return best_move;
    }

    vector<int> available_moves(const vector<char>& board) {
        vector<int> moves;
        for (int i = 0; i < board.size(); ++i) {
            if (board[i] == ' ') {
                moves.push_back(i);
            }
        }
        return moves;
    }

    bool check_winner(const vector<char>& board, char letter) {
        // Winning logic for rows, columns, diagonals
        vector<vector<int>> win_combos = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // rows
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // cols
            {0, 4, 8}, {2, 4, 6}              // diagonals
        };

        for (auto combo : win_combos) {
            if (board[combo[0]] == letter && board[combo[1]] == letter && board[combo[2]] == letter) {
                return true;
            }
        }
        return false;
    }
};

class TicTacToe {
public:
    vector<char> board;
    char current_winner;

    TicTacToe() : board(9, ' '), current_winner(0) {}

    bool make_move(int square, char letter) {
        if (board[square] == ' ') {
            board[square] = letter;
            if (winner(square, letter)) {
                current_winner = letter;
            }
            return true;
        }
        return false;
    }

    bool winner(int square, char letter) {
        // Check if the current move is a winning move
        return (check_row(square, letter) || check_col(square, letter) || check_diagonals(square, letter));
    }

    bool empty_squares() {
        return count(board.begin(), board.end(), ' ') > 0;
    }

    vector<int> available_moves() {
        vector<int> moves;
        for (int i = 0; i < board.size(); ++i) {
            if (board[i] == ' ') {
                moves.push_back(i);
            }
        }
        return moves;
    }

    void print_board() {
        for (int i = 0; i < 9; ++i) {
            if (i % 3 == 0) cout << "\n";
            cout << (board[i] == ' ' ? to_string(i) : string(1, board[i])) << " ";
        }
        cout << "\n";
    }

private:
    bool check_row(int square, char letter) {
        int row_start = (square / 3) * 3;
        return board[row_start] == letter && board[row_start + 1] == letter && board[row_start + 2] == letter;
    }

    bool check_col(int square, char letter) {
        int col_start = square % 3;
        return board[col_start] == letter && board[col_start + 3] == letter && board[col_start + 6] == letter;
    }

    bool check_diagonals(int square, char letter) {
        if (square % 2 == 0) {
            if (board[0] == letter && board[4] == letter && board[8] == letter) return true;
            if (board[2] == letter && board[4] == letter && board[6] == letter) return true;
        }
        return false;
    }
};

int main() {
    TicTacToe game;
    HumanPlayer human('O');
    SmartComputerPlayer computer('X');

    char current_letter = 'X';

    while (game.empty_squares()) {
        game.print_board();
        int square;

        if (current_letter == 'O') {
            square = human.get_move(game.board);
        } else {
            square = computer.get_move(game.board);
        }

        if (game.make_move(square, current_letter)) {
            cout << current_letter << " makes a move to square " << square << endl;
            if (game.current_winner) {
                cout << current_letter << " wins!" << endl;
                break;
            }
            current_letter = (current_letter == 'X') ? 'O' : 'X';
        }
    }

    if (!game.current_winner) {
        cout << "It's a tie!" <<endl;
    }
    return 0;
}

