#include <bits/stdc++.h>

using namespace std;

const int maxn = 101010;

int dx[] = {1, -1, 0, 0};
int dy[] = {0, 0, 1, -1};
int k, n, x, y, val, arr[20], heuristic_type, explored_node, expanded_node;

class node
{
public:
    bool is_root;
    int matrix[4][4];
    int parent[4][4];
    int move_cnt;
    int x, y, hval;

    node()
    {
        is_root = false;
        hval = move_cnt = 0;
    }
};

class set_comparator
{
public:
    bool operator()(const node &a, const node &b) const
    {
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (a.matrix[i][j] < b.matrix[i][j])
                    return true;
                else if (a.matrix[i][j] > b.matrix[i][j])
                    return false;
            }
        }

        return false;
    }
};

class pq_comparator
{
public:
    bool operator()(const node &a, const node &b) const
    {
        if (a.hval < b.hval)
            return false;

        return true;
    }
};

node goal_node;
vector < pair<int, int> > vec;
set <node, set_comparator> st;
priority_queue <node, vector<node>, pq_comparator> pq;

bool isValid(int i, int j)
{
    return (i < n && j < n && i >= 0 && j >= 0);
}

bool isTarget(node cur_node)
{
    val = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            ++val;

            if (val <= k && cur_node.matrix[i][j] != val)
                return false;
        }
    }

    return true;
}

int hamming_distance(node cur_node)
{
    int ret = 0;

    val = 1;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (val <= k)
            {
                if (cur_node.matrix[i][j] != val)
                    ++ret;
            }

            ++val;
        }
    }

    return ret;
}

int manhattan_distance(node cur_node)
{
    int ret = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (val <= k)
                ret += (int) abs(i - vec[cur_node.matrix[i][j] - 1].first) + (int) abs(j - vec[cur_node.matrix[i][j] - 1].second);
        }
    }

    return ret;
}

int linear_conflict(node cur_node)
{
    int ret = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (vec[cur_node.matrix[i][j] - 1].first == i)
            {
                for (int k = j + 1; k < n; ++k)
                {
                    if (cur_node.matrix[i][k] != 0 && vec[cur_node.matrix[i][k] - 1].first == i && cur_node.matrix[i][j] > cur_node.matrix[i][k])
                        ++ret;
                }
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (vec[cur_node.matrix[j][i] - 1].second == i)
            {
                for (int k = j + 1; k < n; ++k)
                {
                    if (cur_node.matrix[k][i] != 0 && vec[cur_node.matrix[k][i] - 1].second == i && cur_node.matrix[j][i] > cur_node.matrix[k][i])
                        ++ret;
                }
            }
        }
    }

    return manhattan_distance(cur_node) + 2 * ret;
}

int heuristic_value(node cur_node)
{
    if (heuristic_type == 1)
        return hamming_distance(cur_node);
    else if (heuristic_type == 2)
        return manhattan_distance(cur_node);
    else
        return linear_conflict(cur_node);
}

int inversion_value(node cur_node)
{
    int ret = 0, idx = 0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (cur_node.matrix[i][j])
                arr[idx++] = cur_node.matrix[i][j];
        }
    }

    for (int i = 0; i <= k; ++i)
    {
        for (int j = i + 1; j <= k; ++j)
        {
            if (arr[i] > arr[j])
                ++ret;
        }
    }

    return ret;
}

bool is_solvable(node cur_node)
{
    if (n % 2 && inversion_value(cur_node) % 2)
        return false;
    else if ((cur_node.x + inversion_value(cur_node) % 2 == 0))
        return false;

    return true;
}

int A_star_search(node start_node)
{
    node cur_node;
    node temp_node = start_node;

    if (is_solvable(start_node))
    {
        pq.push(start_node);
        st.insert(start_node);
    }

    while (!pq.empty())
    {
        ++expanded_node;

        temp_node = pq.top();
        pq.pop();

        if (isTarget(temp_node))
        {
            goal_node= temp_node;
            return temp_node.move_cnt;
        }

        ++expanded_node;

        for (int i = 0; i < 4; ++i)
        {
            x = temp_node.x, y = temp_node.y;

            if (isValid(x + dx[i], y + dy[i]))
            {
                cur_node = temp_node;
                cur_node.is_root = false;
                cur_node.move_cnt += 1;

                for (int i = 0; i < n; ++i)
                {
                    for (int j = 0; j < n; ++j)
                        cur_node.parent[i][j] = temp_node.matrix[i][j];
                }

                cur_node.x = x + dx[i], cur_node.y = y + dy[i];
                swap(cur_node.matrix[x][y], cur_node.matrix[x + dx[i]][y + dy[i]]);

                if (st.count(cur_node) == 0)
                {
                    ++explored_node;

                    cur_node.hval = heuristic_value(cur_node) + cur_node.move_cnt;
                    st.insert(cur_node);
                    pq.push(cur_node);
                }
            }
        }
    }

    return -1;
}

int main()
{
    cin >> k >> heuristic_type, val = 1;

    if (k == 8)
        n = 3;
    else
        n = 4;

    node cur_node;
    cur_node.is_root = true;

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            cin >> cur_node.matrix[i][j];

            if (cur_node.matrix[i][j] == 0)
                cur_node.x = i, cur_node.y = j;

            if (val <= k)
                vec.push_back({i, j});
        }
    }

    cout << "Num of moves : " << A_star_search(cur_node) << endl;
    cout << "Num of expanded node : " << explored_node << endl;
    cout << "Num of explored node : " << expanded_node << endl;

    return 0;
}

