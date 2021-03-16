#include <bits/stdc++.h>

using namespace std;

const int maxn = 101010;

bool mark[111];
int n, m, l, r, temp, best_start_node[2];
double x, y, best_cost, worst_cost, avg_cost, savings[111][111];

deque <int> dq;
vector <int> path;
vector < pair<double, double> > vec;
vector < vector<int> > nnh_path, sh_path;
vector < pair<double, int> > tvec, nnh_val, sh_val;

void check()
{
    bool visit[100];

    memset(visit, false, sizeof(visit));

    for (int i = 0; i < path.size(); ++i)
        cout << path[i] << " ";

    cout << " ---- > " << path.size() << endl;
}

double dist(int i, int j)
{
    double p = vec[i].first - vec[j].first;
    double q = vec[i].second - vec[j].second;

    return sqrt((p * p) + (q * q));
}

double get_cost()
{
    double ret = 0.0;

    for (int i = 1; i < n + 1; ++i)
        ret += dist(path[i - 1], path[i]);

    return ret;
}

void nearest_neighbour_heuristic(int u)
{
    mark[u] = true;
    path.push_back(u);

    int cur_nearest = -1;
    double cur_distance = 1000000000.0;

    for (int i = 0; i < n; ++i)
    {
        if (dist(u, i) < cur_distance && !mark[i])
            cur_distance = dist(u, i), cur_nearest = i;
    }

    if (cur_nearest != -1)
        nearest_neighbour_heuristic(cur_nearest);
    else
        path.push_back(m);
}

void randomize_nearest_neighbour_heuristic(int u)
{
    tvec.clear();
    mark[u] = true;
    path.push_back(u);

    int cur_nearest = -1;
    int cur_distance = 10000000.0;

    for (int i = 0; i < n; ++i)
    {
        if (!mark[i])
            tvec.push_back({dist(i, u), i});
    }

    sort(tvec.begin(), tvec.end());

    if (tvec.size())
        cur_nearest = rand() % (min(5, (int) tvec.size()));

    if (cur_nearest != -1)
        nearest_neighbour_heuristic(cur_nearest);
    else
        path.push_back(m);
}

void calculate_savings()
{
    double temp, cur_max = 0.0;

    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            temp = dist(i, m) + dist(j, m) - dist(i, j);
            savings[i][j] = savings[j][i] = temp;

            if (temp > cur_max && i != m && j != m)
                cur_max = temp, l = i, r = j;
        }
    }
}

void savings_heuristic()
{
    memset(mark, false, sizeof(mark));

    for (int i = 0; i < 111; ++i)
    {
        for (int j = 0; j < 111; ++j)
            savings[i][j] = 0.0;
    }

    calculate_savings();

    int templ, tempr;
    int cur_size = 3;
    double cur_max = 0.0;

    dq.push_front(l);
    dq.push_back(r);
    mark[r] = mark[l] = mark[m] = true;

    while (cur_size < n)
    {
        cur_max = 0.0;
        templ = tempr = -1;

        for (int i = 0; i < n; ++i)
        {
            if (!mark[i] && cur_max < savings[l][i] && i != m)
                cur_max = savings[l][i], templ = i;
        }

        for (int i = 0; i < n; ++i)
        {
            if (!mark[i] && cur_max < savings[r][i] && i != m)
                cur_max = savings[r][i], tempr = i, templ = -1;
        }

        if (templ != -1)
            dq.push_front(templ), l = templ, mark[l] = true;
        else if (tempr != -1)
            dq.push_back(tempr), r = tempr, mark[r] = true;

        cur_size += 1;
    }

    int idx = m;
    path.push_back(m);

    while (!dq.empty())
        path.push_back(dq.front()), dq.pop_front();

    path.push_back(m);
}

void randomize_savings_heuristic()
{
    memset(mark, false, sizeof(mark));

    for (int i = 0; i < 111; ++i)
    {
        for (int j = 0; j < 111; ++j)
            savings[i][j] = 0.0;
    }

    calculate_savings();

    double cur_max = 0.0;
    int tidx[2], cur_size = 3;

    dq.push_front(l);
    dq.push_back(r);
    mark[r] = mark[l] = mark[m] = true;

    vector < pair<double, int> > lvec, rvec;

    while (cur_size < n - 1)
    {
        cur_max = 0.0;
        lvec.clear();
        rvec.clear();

        for (int i = 0; i < n; ++i)
        {
            if (!mark[i] && i != m)
                lvec.push_back({savings[l][i], i});
        }

        sort(lvec.begin(), lvec.end());
        reverse(lvec.begin(), lvec.end());

        if (lvec.size())
            tidx[0] = rand() % min(5, (int) lvec.size());

        for (int i = 0; i < n; ++i)
        {
            if (!mark[i] && i != m)
                rvec.push_back({savings[r][i], i});
        }

        sort(rvec.begin(), rvec.end());
        reverse(rvec.begin(), rvec.end());

        if (rvec.size())
            tidx[1] = rand() % min(5, (int) rvec.size());

        if (tidx[0] != -1 && tidx[1] != -1)
        {
            if (lvec[tidx[0]].first > rvec[tidx[1]].first)
                dq.push_front(lvec[tidx[0]].second), l = lvec[tidx[0]].second, mark[l] = true;
            else
                dq.push_back(rvec[tidx[1]].second), r = rvec[tidx[1]].second, mark[r] = true;
        }
        else if (tidx[0] != -1)
            dq.push_front(lvec[tidx[0]].second), l = lvec[tidx[0]].second, mark[l] = true;
        else
            dq.push_back(rvec[tidx[1]].second), r = rvec[tidx[1]].second, mark[r] = true;

        cur_size += 1;
    }

    int idx = m;
    path.push_back(m);

    while (!dq.empty())
        path.push_back(dq.front()), dq.pop_front();

    path.push_back(m);
}

void first_two_opt_heuristic()
{
    bool isFound;
    double cur_cost;

    cur_cost = get_cost();

    while (true)
    {
        isFound = false;

        for (int i = 0; i < path.size() - 2; ++i)
        {
            for (int j = i + 1; j < path.size() - 1; ++j)
            {
                reverse(path.begin() + i, path.begin() + j + 1);

                if (get_cost() < cur_cost)
                {
                    isFound = true;
                    cur_cost = get_cost();
                    break;
                }
                else
                    reverse(path.begin() + i, path.begin() + j + 1);
            }

            if (isFound)
                break;
        }

        if (!isFound)
            break;
    }
}

void best_two_opt_heuristic()
{
    bool isFound;
    double cur_cost;

    cur_cost = get_cost();
    vector <int> tpath = path, best_path = path;

    while (true)
    {
        isFound = false;

        for (int i = 0; i < path.size() - 2; ++i)
        {
            for (int j = i + 1; j < path.size() - 1; ++j)
            {
                path = tpath;

                reverse(path.begin() + i, path.begin() + j + 1);

                if (get_cost() < cur_cost)
                {
                    isFound = true;
                    cur_cost = get_cost();
                    best_path = path;
                }
            }
        }

        path = best_path;
        tpath = best_path;

        if (!isFound)
            break;
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    srand(time(NULL));

    cin >> n;

    for (int i = 0; i < n; ++i)
        cin >> temp >> x >> y, vec.push_back({x, y});

    vector < int > nnh_task1, sh_task1;

    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //-----------------------------------------Task : 1-------------------------------------
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

    cout << "Task 1" << endl;

    best_cost = 100000000.0;
    worst_cost = avg_cost = 0.0;

    for (int i = 0; i < 5; ++i)
    {
        path.clear();
        memset(mark, false, sizeof(mark));

        m = rand() % n;
        nearest_neighbour_heuristic(m);

        if (get_cost() < best_cost)
            best_cost = get_cost(), nnh_task1 = path, best_start_node[0] = m;

        if (get_cost() > worst_cost)
            worst_cost = get_cost();
        avg_cost += get_cost();
    }

    cout << "Greedy NNH --> Average Cost : " << (avg_cost / 5.0) << " -- Best Cost : " << best_cost << " -- Worst Cost : " << worst_cost << endl;

    best_cost = 100000000.0;
    worst_cost = avg_cost = 0.0;

    for (int i = 0; i < 5; ++i)
    {
        path.clear();

        m = rand() % n;
        savings_heuristic();

        if (get_cost() < best_cost)
            best_cost = get_cost(), sh_task1 = path, best_start_node[1] = m;

        if (get_cost() > worst_cost)
            worst_cost = get_cost();

        avg_cost += get_cost();
    }

    cout << "Greedy SH --> Average Cost : " << (avg_cost / 5.0) << " -- Best Cost : " << best_cost << " -- Worst Cost : " << worst_cost << endl << endl;

    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //-----------------------------------------Task : 2-------------------------------------
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

    cout << "Task 2" << endl;

    avg_cost= 0.0;

    for (int i = 0; i < 10; ++i)
    {
        path.clear();
        memset(mark, false, sizeof(mark));

        randomize_nearest_neighbour_heuristic(best_start_node[0]);

        nnh_val.push_back({get_cost(), i});
        avg_cost += get_cost();
        nnh_path.push_back(path);
    }

    sort(nnh_val.begin(), nnh_val.end());

    cout << "Randomize NNH --> Average Cost : " << (avg_cost / 10.0) << " -- Best Cost : " << nnh_val[0].first << " -- Worst Cost : " << nnh_val[9].first << endl;

    avg_cost = 0.0;

    for (int i = 0; i < 10; ++i)
    {
        path.clear();

        m = best_start_node[1];
        randomize_savings_heuristic();

        sh_val.push_back({get_cost(), i});
        avg_cost += get_cost();
        sh_path.push_back(path);
    }

    sort(sh_val.begin(), sh_val.end());

    cout << "Randomize SH --> Average Cost : " << (avg_cost / 10.0) << " -- Best Cost : " << sh_val[0].first << " -- Worst Cost : " << sh_val[9].first << endl << endl;

    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------
    //-----------------------------------------Task : 3-------------------------------------
    //--------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------

    cout << "Task 3" << endl;

    cout << "First improvement of NNH : ";

    for (int i = 0; i < 4; ++i)
    {
        path = (i < 3 ? nnh_path[nnh_val[i].second] : nnh_task1);
        m = path[0];
        first_two_opt_heuristic();
        cout << get_cost() << "    ";
    }

    cout << endl;

    cout << "First improvement of SH : ";

    for (int i = 0; i < 4; ++i)
    {
        path = (i < 3 ? sh_path[sh_val[i].second] : sh_task1);
        m = path[0];
        first_two_opt_heuristic();
        cout << get_cost() << "    ";
    }

    cout << endl << endl;

    cout << "Best improvement NNH : ";

    for (int i = 0; i < 4; ++i)
    {
        path = (i < 3 ? nnh_path[nnh_val[i].second] : nnh_task1);
        m = path[0];
        best_two_opt_heuristic();
        cout << get_cost() << "    ";
    }

    cout << endl;

    cout << "Best improvement SH : ";

    for (int i = 0; i < 4; ++i)
    {
        path = (i < 3 ? sh_path[sh_val[i].second] : sh_task1);
        m = path[0];
        best_two_opt_heuristic();
        cout << get_cost() << "    ";
    }

    cout << endl;

    return 0;
}