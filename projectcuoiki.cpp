#include<iostream> 
#include <list> 
#include<vector>
#include<queue>
#include<chrono>

using namespace std;


struct Node
{
    int first;
    int second;
    int wait1;
    int time1;
    int wait2;
    int time2;
    int pen;
    int tinh;
    int depth;
    struct Node* parent;
    struct Node* child;
};

//So sanh de them node vao priority queue
struct NodeCmp
{
    bool operator()(const Node* ln, const Node* rn) const
    {
        return ln->tinh > rn->tinh;
    }
};

class Graph
{
    int V;
    vector<pair<int,int>>* adj;
    int** time;
    int** edge;
public:
    Graph(int V);
    void addEdge(int v, int w,int weight);
    void Table();
    Node* bestFirstSearch(int start, int goal);
    Node* aStar(int firstP, int secondP);
    Node* BFS(int start, int goal);
    int minTimeBFS(int u, int v);
    int minEdgeBFS(int u, int v);
    int h1(int f, int s);
    int h2(int f, int s);
    void print();
};

void Graph::print()
{
    for (int i = 0; i < V - 1; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            if(time[i][j] != 0)
                cout << i << " "<< j << " " << time[i][j] << endl;
        }
    }
}
void Graph::Table()
{
    for (int i = 0; i < V-1; i++)
    {
        for (int j = i + 1; j < V; j++)
        {
            time[i][j] = minEdgeBFS(i, j);
            time[j][i] = time[i][j];
            edge[i][j] = minTimeBFS(i, j);
            edge[j][i] = edge[i][j];
        }
    }
}

int Graph::h1(int f, int s)
{
    return time[f][s]/2;
}

// Khoi tao graph
Graph::Graph(int V)
{
    this->V = V;
    adj = new vector<pair<int,int>>[V];
    time = new int*[V];
    edge = new int* [V];
    for (int i = 0; i < V; i++)
    {
        time[i] = new int[V];
        edge[i] = new int[V];
    }
}
// them canh noi dinh v va w
void Graph::addEdge(int v, int w, int weight)
{
    adj[v].push_back(make_pair(w,weight));
    adj[w].push_back(make_pair(v, weight));
}

int Graph::minTimeBFS(int u, int v)
{
    vector<bool> visited(V, 0);
    vector<int> distance(V, 0);
    queue <int> q;
    distance[u] = 0;

    q.push(u);
    visited[u] = true;
    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        for (auto i = adj[x].begin(); i != adj[x].end(); i++)
        {
            if (visited[i->first])
                continue;
            distance[i->first] = distance[x] + i->second;
            q.push(i->first);
            visited[i->first] = true;
        }
    }
    return distance[v];
}
int Graph::minEdgeBFS(int u, int v)
{
    vector<bool> visited(V, 0);
    vector<int> distance(V, 0);
    queue <int> q;
    distance[u] = 0;

    q.push(u);
    visited[u] = true;
    while (!q.empty())
    {
        int x = q.front();
        q.pop();
        for (auto i = adj[x].begin(); i != adj[x].end(); i++)
        {
            if (visited[i->first])
                continue;
            distance[i->first] = distance[x] + 1;
            q.push(i->first);
            visited[i->first] = true;
        }
    }
    return distance[v];
}

// tao node con
Node* Child(Node* parent, int first)
{
    Node* n = new Node();
    n->parent = parent;
    n->child = NULL;
    n->first = first;
    return n;
}
//kiem tra value co nam trong queue
bool inQueue(priority_queue <Node*, vector<Node*>, NodeCmp> pq,
    int first, int second)
{
    priority_queue <Node*, vector<Node*>, NodeCmp> temp = pq;
    while (!temp.empty())
    {
        Node* t = temp.top();
        temp.pop();
        if (t->first == first && t->second == second)
            return true;
    }
    return false;
}

bool inList(list<Node*> ls,
    int first, int second)
{
    list<Node*> temp = ls;
    while (!temp.empty())
    {
        Node* t = temp.front();
        temp.pop_front();
        if (t->first == first && t->second == second)
            return true;
    }
    return false;
}

Node* Graph::bestFirstSearch(int firstP, int secondP)
{
    Node* n = new Node();
    n->first = firstP;
    n->second = secondP;
    n->time1 = 0;
    n->time2 = 0;
    if (firstP == secondP)
    {
        return n;
    }
    priority_queue <Node*, vector<Node*>, NodeCmp> open;
    open.push(n);
    priority_queue <Node*, vector<Node*>, NodeCmp> close;

    while (n->first != n->second && !open.empty())
    {
        n = open.top();
        open.pop(); 
        if (n->first == n->second)
        {
            return n;
        }
        for (auto i = adj[n->first].begin(); i != adj[n->first].end(); i++)
        {
            for (auto j = adj[n->second].begin(); j != adj[n->second].end(); j++)
            if (!inQueue(open, i->first, j->first) 
                && !inQueue(close, i->first, j->first))
            {
                Node* t = new Node();
                t->parent = n;
                t->first = i->first;
                t->second = j->first;
                t->time1 = t->parent->time1 + i->second;
                t->time2 = t->parent->time2 + j->second;
                int ss = i->second - j->second;
                if (ss > 0)
                {
                    t->wait2 = ss;
                }
                else
                {
                    t->wait1 = abs(ss);
                }
                t->tinh = t->wait1 + t->time1 + t->wait2 + t->time2;
                open.push(t);
               /* Node* fw = new Node();
                fw->parent = n;
                fw->first = n->first;
                fw->second = j->first;
                fw->time2 = n->time2 + j->second;
                fw->time1 = n->time1;
                fw->wait1 = j->second;
                fw->wait2 = n->wait2;
                fw->pen = 125;
                fw->tinh = fw->wait1 + fw->time1 + fw->wait2 + fw->time2 + fw->pen;
                open.push(fw);
                Node* sw = new Node();
                sw->parent = n;
                sw->first = i->first;
                sw->second = n->second;
                sw->time1 = n->time1 + i->second;
                sw->time2 = n->time2;
                sw->wait2 = i->second;
                sw->wait1 = n->wait1;
                sw->pen = 125;
                sw->tinh = sw->wait1 + sw->time1 + sw->wait2 + sw->time2 + sw->pen;
                open.push(sw);*/
            }
        }
        close.push(n);
    }
    return NULL;
}

Node* Graph::aStar(int firstP, int secondP)
{
    Node* n = new Node();
    n->first = firstP;
    n->second = secondP;
    n->time1 = 0;
    n->time2 = 0;
    if (firstP == secondP)
    {
        return n;
    }
    priority_queue <Node*, vector<Node*>, NodeCmp> open;
    open.push(n);
    priority_queue <Node*, vector<Node*>, NodeCmp> close;

    while (n->first != n->second && !open.empty())
    {
        n = open.top();
        open.pop();
        if (n->first == n->second)
        {
            return n;
        }
        for (auto i = adj[n->first].begin(); i != adj[n->first].end(); i++)
        {
            for (auto j = adj[n->second].begin(); j != adj[n->second].end(); j++)
                if (!inQueue(open, i->first, j->first)
                    && !inQueue(close, i->first, j->first))
                {
                    Node* t = new Node();
                    t->parent = n;
                    t->first = i->first;
                    t->second = j->first;
                    t->time1 = t->parent->time1 + i->second;
                    t->time2 = t->parent->time2 + j->second;
                    int ss = i->second - j->second;
                    if (ss > 0)
                    {
                        t->wait2 = ss;
                    }
                    else
                    {
                        t->wait1 = abs(ss);
                    }
                    t->tinh = t->wait1 + t->time1 + t->wait2 + t->time2 + h1(t->first, t->second);
                    open.push(t);
                }
        }
        close.push(n);
    }
    return NULL;
}

Node* Graph::BFS(int firstP, int secondP)
{
    Node* n = new Node();
    n->first = firstP;
    n->second = secondP;
    n->time1 = 0;
    n->time2 = 0;
    if (firstP == secondP)
    {
        return n;
    }
    list<Node*> open;
    open.push_front(n);
    list<Node*> close;

    while (n->first != n->second && !open.empty())
    {
        n = open.front();
        open.pop_front();
        if (n->first == n->second)
        {
            return n;
        }
        for (auto i = adj[n->first].begin(); i != adj[n->first].end(); i++)
        {
            for (auto j = adj[n->second].begin(); j != adj[n->second].end(); j++)
                if (!inList(open, i->first, j->first)
                    && !inList(close, i->first, j->first))
                {
                    Node* t = new Node();
                    t->parent = n;
                    t->first = i->first;
                    t->second = j->first;
                    t->time1 = t->parent->time1 + i->second;
                    t->time2 = t->parent->time2 + j->second;
                    int ss = i->second - j->second;
                    if (ss > 0)
                    {
                        t->wait2 = ss;
                    }
                    else
                    {
                        t->wait1 = abs(ss);
                    }
                    open.push_back(t);
                }
        }
        close.push_front(n);
    }
    return NULL;
}

void printPath(Node* n)
{
    if (n == NULL)
    {
        cout << "Khong tim duoc duong di";
        return;
    }
    Node* temp1 = new Node();
    list<Node*> pathF;
    list<Node*> pathS;
    temp1 = n;
    int thoigiandi1 = 0;
    int thoigiandi2 = 0;
    int thoigiancho1 = 0;
    int thoigiancho2 = 0;
    pathF.push_front(temp1);
    thoigiandi1 = temp1->time1;
    thoigiandi2 = temp1->time2;
    thoigiancho1 += temp1->wait1;
    thoigiancho2 += temp1->wait2;
    temp1 = temp1->parent;
    while (temp1 != NULL)
    {
        thoigiancho1 += temp1->wait1;
        thoigiancho2 += temp1->wait2;
        pathF.push_front(temp1);
        temp1 = temp1->parent;
    }
    pathS = pathF;
    cout << "Duong di cua nguoi thu nhat: ";
    while (!pathF.empty())
    {
        cout << pathF.front()->first << " ";
        pathF.pop_front();
    }
    cout << endl << "Duong di cua nguoi thu hai: ";
    while (!pathS.empty())
    {
        cout << pathS.front()->second << " ";
        pathS.pop_front();
    }
    cout << endl << endl;
    cout << "Thoi gian nguoi thu nhat di: " << thoigiandi1 << endl;
    cout << "Thoi gian nguoi thu hai di: " << thoigiandi2 << endl << endl;

    cout << "Thoi gian nguoi thu nhat cho: " << thoigiancho1 << endl;
    cout << "Thoi gian nguoi thu hai cho: " << thoigiancho2 << endl << endl;

    cout << "Tong thoi gian: " << thoigiandi1 + thoigiancho1 << endl;

}

int main()
{
    enum Ten
    {
        Oradea=1,
        Zerind,
        Arad,
        Timisoara,
        Lugoj,
        Mehadia,
        Drobeta,
        Craiova,
        Pitesti,
        Rimnicu,
        Vilcea,
        Siubiu,
        Fagaras,
        Bucharest,
        Giurgiu,
        Urziceni,
        Hirsova,
        Eforie,
        Vaslui,
        Isasi,
        Neamt
    };
    Graph g(21);
    g.addEdge(1, 2, 71);
    g.addEdge(1, 11, 151);
    g.addEdge(2, 3, 75);
    g.addEdge(3, 11, 140);
    g.addEdge(3, 4, 118);
    g.addEdge(4, 5, 111);
    g.addEdge(5, 6, 70);
    g.addEdge(6, 7, 75);
    g.addEdge(7, 8, 120);
    g.addEdge(8, 9, 138);
    g.addEdge(9, 10, 97);
    g.addEdge(10, 8, 146);
    g.addEdge(11, 10, 80);
    g.addEdge(11, 12, 99);
    g.addEdge(9, 13, 101);
    g.addEdge(12, 13, 211);
    g.addEdge(13, 14, 90);
    g.addEdge(13, 15, 85);
    g.addEdge(15, 16, 98);
    g.addEdge(16, 17, 86);
    g.addEdge(15, 18, 142);
    g.addEdge(18, 19, 92);
    g.addEdge(19, 20, 87);
    g.Table();
    int f, s;
    char st;
    start:
    cout << "Nhap vi tri nguoi thu nhat va thu hai: ";
    cin >> f >> s;
    while (f < 0 || f>20 || s<0 || s>20)
    {
        cout << "Nhap sai dinh!" << endl;
        cout << "Nhap vi tri nguoi thu nhat va thu hai: ";
        cin >> f >> s;
    }
    cout << endl;
    auto start = chrono::high_resolution_clock::now();
    printPath(g.bestFirstSearch(f, s));
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Thoi gian chay thuat toan best first: " << duration.count() << " ms" << endl << endl;
    cout << "--------------------------------------------------------" << endl;
    start = chrono::high_resolution_clock::now();
    printPath(g.aStar(f, s));
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Thoi gian chay thuat toan a*: " << duration.count() << " ms" << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "\nTiep tuc? Y/N ";
    cin >> st;
    switch (st)
    {
    case 'y':
        goto start;
        break;
    case 'Y':
        goto start;
        break;
    default:
        break;
    }

    return 0;
}