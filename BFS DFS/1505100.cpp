#include <bits/stdc++.h>

using namespace std;

const int maxn = 101010;

bool isTimeOut;
time_t t;
pair<int, pair<int, int>> des_node;
int n, m, k, num_of_node_explored;
set < pair<int, pair<int, int>> > st;
queue < pair<int, pair<int, int>> > q;
vector < pair<int, pair<int, int>> > vec;
map < pair<int, pair<int, int>>, pair<int, pair<int, int>> > mp;

void print_path(pair<int, pair<int, int>> var)
{
    while (mp.count(var))
    {
        if (var.first)
            cout << "DES M" << n - var.second.first << "/C" << m - var.second.second << "----->" << "SRC M" << var.second.first << "/C" << var.second.second << endl;
        else
           cout << "SRC M" << var.second.first << "/C" << var.second.second << "----->" << "DES M" << n - var.second.first << "/C" << m - var.second.second << endl;
        
        var = mp[var];
    }
    
    cout << "SRC M" << var.second.first << "/C" << var.second.second << "----->" << "DES M" << n - var.second.first << "/C" << m - var.second.second << endl;
}

bool bfs(int missionary_cnt, int cannibal_cnt, int allowable_passenger)
{
    pair<int, pair<int, int>> temp;
    
    temp.first = 0;
    temp.second.first = missionary_cnt;
    temp.second.second = cannibal_cnt;
    q.push(temp);
    st.insert(temp);
    
    while (!q.empty())
    {
        ++num_of_node_explored;
        
        if (((double) (clock() - t) / CLOCKS_PER_SEC) > 10.0)
        {
            isTimeOut = true;
            return false;
        }
        
        if (num_of_node_explored > 10 * maxn)
            break;
        
        temp = q.front();
        q.pop();
        
        if (temp.second.first == 0 && temp.second.second == 0)
        {
            print_path(temp);
            return true;
        }
        
        int destination_missionary_cnt = n - temp.second.first;
        int destination_cannibal_cnt = m - temp.second.second;
        
        if ((temp.second.first && temp.second.first < temp.second.second) || (destination_missionary_cnt < destination_cannibal_cnt && destination_missionary_cnt))
            continue;
        
        int cur_bank_missionary_cnt = (temp.first ? (missionary_cnt - temp.second.first) : temp.second.first);
        int cur_bank_cannibal_cnt = (temp.first ? (cannibal_cnt - temp.second.second) : temp.second.second);
        
        for (int i = 0; i <= min(cur_bank_missionary_cnt, allowable_passenger); ++i)
        {
            for (int j = 0; j <= min(cur_bank_cannibal_cnt, allowable_passenger); ++j)
            {
                if (i == 0 && j == 0)
                    continue;
                    
                if (i + j <= allowable_passenger)
                {
                    pair<int, pair<int, int>> var;
                    
                    if (temp.first)
                    {
                        var.first = 0;
                        var.second.first = temp.second.first + i;
                        var.second.second = temp.second.second + j;
                        
                        if (!st.count(var))
                        {
                            mp[var] = temp;
                            q.push(var);
                            st.insert(var);
                        }
                    }
                    else
                    {
                        var.first = 1;
                        var.second.first = temp.second.first - i;
                        var.second.second = temp.second.second - j;
                        
                        if (!st.count(var))
                        {
                            mp[var] = temp;
                            q.push(var);
                            st.insert(var);
                        }
                    }
                }
            }
        }
    }
    
    return false;
}

bool dfs(int boat_dir, int missionary_cnt, int cannibal_cnt)
{
    ++num_of_node_explored;
    
    if (((double) (clock() - t) / CLOCKS_PER_SEC) > 10.0)
    {
        isTimeOut = true;
        return false;
    }
    
    if (num_of_node_explored > 10 * maxn)
        return false;
    
    bool ret = false;
    pair<int, pair<int, int>> temp;
    
    if (missionary_cnt == 0 && cannibal_cnt == 0)
    {
        des_node = make_pair(boat_dir, make_pair(missionary_cnt, cannibal_cnt));
        return true;
    }
    
    int destination_missionary_cnt = n - missionary_cnt;
    int destination_cannibal_cnt = m - cannibal_cnt;
    
    if ((missionary_cnt < cannibal_cnt && missionary_cnt) || (destination_missionary_cnt < destination_cannibal_cnt && destination_missionary_cnt))
        return ret;
    
    int cur_bank_missionary_cnt = (boat_dir ? (n - missionary_cnt) : missionary_cnt);
    int cur_bank_cannibal_cnt = (boat_dir ? (m - cannibal_cnt) : cannibal_cnt);
    
    for (int i = 0; i <= min(cur_bank_missionary_cnt, k); ++i)
    {
        for (int j = 0; j <= min(cur_bank_cannibal_cnt, k); ++j)
        {
            if (i == 0 && j == 0)
                continue;
            
            if (i + j <= k)
            {
                if (boat_dir)
                {
                    temp.first = 0;
                    temp.second.first = missionary_cnt + i;
                    temp.second.second = cannibal_cnt + j;
                    
                    if (!st.count(temp))
                    {
                        mp[temp] = make_pair(boat_dir, make_pair(missionary_cnt, cannibal_cnt));
                        st.insert(temp);
                        ret |= dfs(temp.first, temp.second.first, temp.second.second);
                    }
                }
                else
                {
                    temp.first = 1;
                    temp.second.first = missionary_cnt - i;
                    temp.second.second = cannibal_cnt - j;
                    
                    if (!st.count(temp))
                    {
                        mp[temp] = make_pair(boat_dir, make_pair(missionary_cnt, cannibal_cnt));
                        st.insert(temp);
                        ret |= dfs(temp.first, temp.second.first, temp.second.second);
                    }
                }
            }
        }
    }
    
    return ret;
}

int main()
{
    cin >> n >> m >> k;
    
    t = clock();
    
    if (bfs(n, m, k))
        cout << "YES " << num_of_node_explored << endl;
    else
    {
        cout << "NO " << num_of_node_explored << endl;
        
        if (isTimeOut)
            cout << "Timed Out" << endl;
    }
    
    cout << "Time in BFS " << (double) (clock() - t) / CLOCKS_PER_SEC << endl;
    
    st.clear();
    num_of_node_explored = 0;
    isTimeOut = false;
    mp.clear();
    t = clock();
    
    if (dfs(0, n, m))
        cout << "YES " << num_of_node_explored << endl;
    else
    {
        cout << "NO " << num_of_node_explored << endl;
        
        if (isTimeOut)
            cout << "Timed Out" << endl;
    }
    
    cout << "Time in DFS " << (double) (clock() - t) / CLOCKS_PER_SEC << endl;
    
    return 0;
}
