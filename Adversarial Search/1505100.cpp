#include <bits/stdc++.h>

using namespace std;

const int maxn = 101010;
const int max_depth = 9;
const int inf = 100000000;
const int max_game_cnt = 2000;

bool choose;
int extra_move_cnt;
int turn, var, type, w1, w2, w3, w4;

class stat
{
public:
    int w1, w2, w3, w4, t;
};

vector <stat> vec[3];

class board
{
public:
    int arr[2][6];
    int store[2];

    board()
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 6; ++j)
                arr[i][j] = 4;
        }

        store[0] = store[1] = 0;
    }

    void set_value(board b)
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 6; ++j)
                arr[i][j] = b.arr[i][j];
        }

        store[0] = b.store[0];
        store[1] = b.store[1];
    }

    bool update(int turn, int idx, bool is_true)
    {
        bool extra_move = false, is_store = is_true;
        int cur_turn = turn, stone_cnt = arr[turn][idx];
        arr[turn][idx] = 0;

        if (!is_true)
        {
            if (turn)
                ++idx;
            else
                --idx;
        }

        while (stone_cnt)
        {
            --stone_cnt;

            if (is_store)
            {
                store[turn] += 1;

                if (stone_cnt == 0)
                    extra_move = true;
            }
            else
            {
                if (stone_cnt == 0 && cur_turn == turn && arr[cur_turn][idx] == 0)
                    store[cur_turn] += arr[!cur_turn][idx] + 1, arr[!cur_turn][idx] = 0;
                else
                    arr[cur_turn][idx] += 1;
            }

            if (cur_turn && !is_store)
            {
                if (idx < 5)
                    ++idx;
                else
                {
                    if (stone_cnt && cur_turn == turn)
                        is_store = true;
                    else
                        cur_turn = !cur_turn;
                }
            }
            else if (!is_store)
            {
                if (idx > 0)
                    --idx;
                else
                {
                    if (stone_cnt && cur_turn == turn)
                        is_store = true;
                    else
                        cur_turn = !cur_turn;
                }
            }
            else
                is_store = false, cur_turn = !cur_turn;
        }

        return extra_move;
    }

    bool game_over()
    {
        int val1 = 0, val2 = 0;

        for (int i = 0; i < 6; ++i)
            val1 += arr[0][i], val2 += arr[1][i];

        if (val1 == 0)
            store[1] += val2;

        if (val2 == 0)
            store[0] += val1;

        if (store[0] + store[1] == 48)
            return true;

        return false;
    }

    int result()
    {
        if (store[0] > store[1])
            return 0;
        else if (store[1] > store[0])
            return 1;
        else
            return 2;
    }
};

board game;
int maximizer(int depth, int alpha, int beta, int turn, board cur_board);
int minimizer(int depth, int alpha, int beta, int turn, board cur_board);
int alpha_beta_pruning(int depth, int alpha, int beta, int turn, bool isMax, board cur_board);

int heuristic_one(int turn, board cur_board)
{
    return cur_board.store[turn] - cur_board.store[!turn];
}

int heuristic_two(int turn, board cur_board)
{
    int stone_on_cur_side = 0, stone_on_opposite_side = 0;

    for (int i = 0; i < 6; ++i)
        stone_on_cur_side += cur_board.arr[turn][i], stone_on_opposite_side += cur_board.arr[!turn][i];

    return (w1 * heuristic_one(turn, cur_board) + w2 * (stone_on_cur_side - stone_on_opposite_side));
}

int heuristic_three(int turn, board cur_board)
{
    return heuristic_two(turn, cur_board) + w3 * extra_move_cnt;
}

int heuristic_four(int turn, board cur_board)
{
    return heuristic_three(turn, cur_board) + w4 * (cur_board.store[turn] - game.store[turn]);
}

int get_heuristic(int turn, board cur_board)
{
    int ret;

    if (type == 1)
        return heuristic_one(turn, cur_board);
    else if (type == 2)
        return heuristic_two(turn, cur_board);
    else if (type == 3)
        return heuristic_three(turn, cur_board);
    else
        return heuristic_four(turn, cur_board);

    return ret;
}

int alpha_beta_pruning(int depth, int alpha, int beta, int turn, bool isMax, board cur_board)
{
    if (cur_board.game_over())
    {
        if (cur_board.result() == turn)
            return maxn;
        else if (cur_board.result() == 2)
            return get_heuristic(turn, cur_board);
        else
            return -maxn;
    }

    if (!depth)
        return get_heuristic(turn, cur_board);

    if (isMax)
        return maximizer(depth, alpha, beta, turn, cur_board);
    else
        return minimizer(depth, alpha, beta, turn, cur_board);
}

int maximizer(int depth, int alpha, int beta, int turn, board cur_board)
{
    board temp;
    bool isTrue = false;
    int val, idx, next_move = -1, cur_max = -inf;

    temp.set_value(cur_board);

    for (int i = 0; i < 6; ++i)
    {
        if (cur_board.arr[turn][i])
        {
            if (i < 5)
                isTrue = false, idx = i + 1;
            else
                isTrue = true, idx = i;

            if (cur_board.update(turn, idx, isTrue))
            {
                ++extra_move_cnt;
                val = alpha_beta_pruning(depth - 1, alpha, beta, turn, true, cur_board);
                --extra_move_cnt;
            }
            else
                val = alpha_beta_pruning(depth - 1, alpha, beta, turn, false, cur_board);

            if (val > cur_max)
                cur_max = val, next_move = i;

            alpha = max(alpha, cur_max);

            if (beta <= alpha)
                break;

            cur_board.set_value(temp);
        }
    }

    if (depth == max_depth)
        return next_move;

    return cur_max;
}

int minimizer(int depth, int alpha, int beta, int turn, board cur_board)
{
    board temp;
    bool isTrue = false;
    int val, idx, cur_min = inf;

    temp.set_value(cur_board);

    for (int i = 0; i < 6; ++i)
    {
        if (cur_board.arr[!turn][i])
        {
            if (i > 0)
                isTrue = false, idx = i - 1;
            else
                isTrue = true, idx = i;

            if (cur_board.update(!turn, idx, isTrue))
            {
                --extra_move_cnt;
                val = alpha_beta_pruning(depth - 1, alpha, beta, turn, false, cur_board);
                ++extra_move_cnt;
            }
            else
                val = alpha_beta_pruning(depth - 1, alpha, beta, turn, true, cur_board);

            cur_min = min(cur_min, val);
            beta = min(alpha, cur_min);

            if (beta <= alpha)
                break;

            cur_board.set_value(temp);
        }
    }

    return cur_min;
}

int main()
{
    int game_cnt = 0;
    int res[3] = {0, 0, 0};

    board new_board;

    for (int t = 4; t >= 0; --t)
    {
        for (int i = 50; i >= 50; --i)
        {
            for (int j = 1; j <= 50; ++j)
            {
                for (int k = 25; k >= 25; --k)
                {
                    for (int l = 1; l <= 25; ++l)
                    {
                        game.set_value(new_board);

                        extra_move_cnt = 0;
                        turn = 0;
                        w1 = i;
                        w2 = j;
                        w3 = k;
                        w4 = l;
                        type = t;

                        while (!game.game_over())
                        {
                            choose = false;
                            var = alpha_beta_pruning(max_depth, -inf, inf, turn, true, game);

                            if (!turn && var == 0)
                                choose = true;

                            if (turn && var == 5)
                                choose = true;

                            if (!game.update(turn, var, choose))
                                turn = !turn;
                        }

                        stat st;
                        st.w1 = w1, st.w2 = w2, st.w3 = w3, st.w4 = w4, st.t = t;
                        vec[game.result()].push_back(st);
                        ++game_cnt;

                        if (game_cnt == max_game_cnt)
                            break;
                    }

                    if (game_cnt == max_game_cnt)
                        break;
                }

                if (game_cnt == max_game_cnt)
                    break;
            }

            if (game_cnt == max_game_cnt)
                break;
        }

        if (game_cnt == max_game_cnt)
            break;
    }

    cout << "Win ratio for player 1: " << (vec[0].size() / (double) max_game_cnt) << endl;
    cout << "Win ratio for player 2: " << (vec[1].size() / (double) max_game_cnt) << endl;

    return 0;
}