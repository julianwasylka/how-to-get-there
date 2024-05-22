#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

using namespace std;

template <class T> class Vector
{
private:
    size_t _size;
    size_t _capacity;
    T* _data;

public:
    Vector() : _size(0), _capacity(4), _data(new T[_capacity]) {}

    Vector(size_t size) : _size(size), _capacity(size * 2), _data(new T[_capacity]) {}

    ~Vector()
    {
        delete[] _data;
    }

    void push_back(const T& value)
    {
        if (_size == _capacity)
        {
            reserve(_capacity * 2);
        }
        _data[_size] = value;
        ++_size;
    }

    void reserve(size_t capacity)
    {
        if (capacity > _capacity)
        {
            T* new_data = new T[capacity];
            for (size_t i = 0; i < _size; ++i)
            {
                new_data[i] = _data[i];
            }
            delete[] _data;
            _data = new_data;
            _capacity = capacity;
        }
    }

    size_t size() const
    {
        return _size;
    }
    T& operator[](size_t index)
    {
        return _data[index];
    }

    const T& operator[](size_t index) const
    {
        return _data[index];
    }
};

struct Point {
    int x, y, d;
    Point(int x = 0, int y = 0, int d = 0) : x(x), y(y), d(d) {}
};

class IntPairHashMap {
private:
    int TABLE_SIZE;
    struct Entry {
        Point key;
        int value;
        Entry* next;
    };
    Vector<Entry*> table;

    int hash(Point& key)
    {
        return (key.x ^ (key.y << 1)) % TABLE_SIZE;
    }

public:
    IntPairHashMap(int TABLE_SIZE = 128) {
        this->TABLE_SIZE = TABLE_SIZE;
        table.reserve(TABLE_SIZE);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i] = nullptr;
        }
    }

    ~IntPairHashMap() {
        for (int i = 0; i < table.size(); i++) {
            while (table[i] != nullptr) {
                Entry* next = table[i]->next;
                delete table[i];
                table[i] = next;
            }
        }
    }

    // Adds a key-value pair to the map
    void put(Point key, int value) {
        int h = hash(key);
        Entry* e = table[h];
        while (e != nullptr) {
            if (e->key.x == key.x && e->key.y == key.y) {
                e->value = value;
                return;
            }
            e = e->next;
        }

        Entry* newEntry = new Entry{ key, value, table[h] };
        table[h] = newEntry;
    }

    // Gets the value associated with the key, or returns -1 if the key is not in the map
    int get(Point key) {
        int h = hash(key);
        Entry* e = table[h];
        while (e != nullptr) {
            if (e->key.x == key.x && e->key.y == key.y)
            {
                return e->value;
            }
            e = e->next;
        }
        return -1;
    }
};

class CharPtrHashMap {
private:
    int TABLE_SIZE = 0;
    struct Entry {
        char* key;
        int value;
        Entry* next;
    };
    Vector<Entry*> table;

    // Funkcja haszująca dla char*
    int hash(const char* key)
    {
        int hash = 0;
        for (const char* p = key; *p != '\0'; ++p) {
            hash = (hash * 31 + static_cast<int>(*p)) % TABLE_SIZE;
        }
        return hash;
    }

public:
    CharPtrHashMap(int TABLE_SIZE = 128) {
        this->TABLE_SIZE = TABLE_SIZE;
        table.reserve(TABLE_SIZE);
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            table[i] = nullptr;
        }
    }

    ~CharPtrHashMap() {
        for (int i = 0; i < table.size(); i++) {
            while (table[i] != nullptr) {
                Entry* next = table[i]->next;
                //delete[] table[i]->key;
                delete table[i];
                table[i] = next;
            }
        }
    }

    // Dodaje klucz i przypisany mu indeks do mapy
    void put(const char* key, int value) {
        int h = hash(key);
        Entry* e = table[h];
        while (e != nullptr) {
            if (strcmp(e->key, key) == 0) {
                e->value = value;
                return;
            }
            e = e->next;
        }

        Entry* newEntry = new Entry{ (char*)key, value, table[h] };
        table[h] = newEntry;
    }

    // Pobiera wartość przypisaną do klucza lub zwraca -1, jeśli klucz nie istnieje
    int get(const char* key) {
        int h = hash(key);
        Entry* e = table[h];
        while (e != nullptr) {
            if (strcmp(e->key, key) == 0) {
                return e->value;
            }
            e = e->next;
        }
        return -1;
    }
};

class Queue
{
private:
    struct Node
    {
        Point point;
        Node* next;
        Node(const Point& point) : point(point), next(nullptr) {}
    };
    Node* head_;
    Node* tail_;

public:
    Queue()
    {
        head_ = nullptr;
        tail_ = nullptr;
    }

    ~Queue()
    {
        while (head_ != nullptr)
        {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    void push(Point point) {
        Node* newNode = new Node(point);
        if (tail_ == nullptr) {
            head_ = newNode;
            tail_ = newNode;
        }
        else {
            tail_->next = newNode;
            tail_ = newNode;
        }
    }

    Point pop() {
        if (head_ == nullptr) {
            throw std::out_of_range("Queue is empty");
        }
        Point point = head_->point;
        Node* temp = head_;
        head_ = head_->next;
        delete temp;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
        return point;
    }

    bool empty() const {
        return head_ == nullptr;
    }
};

struct Path
{
    int time;
    int targetId;

    Path(int i = 0, int j = 0) : time(i), targetId(j) {};

    friend bool operator>(const Path& first, const Path& second)
    {
        return first.time > second.time;
    }
};

class PriorityQueue {
private:
    struct Node {
        Path value;
        int priority;
        Node* next;
    };
    Node* head;

public:
    PriorityQueue() : head(nullptr) {}

    ~PriorityQueue() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void enqueue(Path value, int priority) {
        Node* newNode = new Node{ value, priority, nullptr };
        if (head == nullptr || priority > head->priority) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next != nullptr && priority <= current->next->priority) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    Path dequeue() {
        if (head == nullptr) {
            throw out_of_range("PriorityQueue is empty");
        }
        Node* temp = head;
        Path value = temp->value;
        head = head->next;
        delete temp;
        return value;
    }

    bool isEmpty() const {
        return head == nullptr;
    }
};

struct City
{
    char* name;
    int row;
    int column;
};

struct Flight
{
    char* dep;
    char* dest;
    int time;
};

struct Request
{
    char* dep;
    char* dest;
    int mode;   //0 - czas   1 - czas i droga
};

struct Board
{
    char field;
    bool visited = false;
};

struct Map
{
    Board** map = nullptr;
    Vector<City> cities;
    CharPtrHashMap* mapNameCityIndex = nullptr;
    IntPairHashMap* mapPointCityIndex = nullptr;
    Vector<Path>* graph = nullptr;
    //Vector<Flight> flights;
    Vector<Request> requests;
    int w = 0;
    int h = 0;
    int k = 0;
    int q = 0;

    void readFlights()
    {
        cin >> k;
        //flights.reserve(k);
        int c = _getchar_nolock();
        int j = 0;
        char dep[20] = { 0 };
        char dest[20] = { 0 };
        char buf[20] = { 0 };
        for (int i = 0; i < k; i++) {
            while (c != ' ')
            {
                c = _getchar_nolock();
                dep[j] = c;
                j++;
            }
            dep[j - 1] = '\0';
            j = 0;
            do
            {
                c = _getchar_nolock();
                dest[j] = c;
                j++;
            } while (c != ' ');
            dest[j - 1] = '\0';
            j = 0;
            while (c != '\n')
            {
                c = _getchar_nolock();
                buf[j] = c;
                j++;
            }
            buf[j - 1] = '\0';
            int time = atoi(buf);
            j = 0;
            //cin >> dep >> dest >> time;
            int depIndex = mapNameCityIndex->get(dep);
            int destIndex = mapNameCityIndex->get(dest);
            graph[depIndex].push_back({ destIndex, time });
            //flights.push_back({ cities[depIndex].name, cities[destIndex].name, time });
        }
    }

    void readRequests()
    {
        cin >> q;
        int mode = 0;
        char dep[20] = { 0 };
        char dest[20] = { 0 };
        for (int i = 0; i < q; i++) {
            cin >> dep >> dest >> mode;
            int depIndex = mapNameCityIndex->get(dep);
            int destIndex = mapNameCityIndex->get(dest);
            requests.push_back({ cities[depIndex].name, cities[destIndex].name, mode });
        }
    }

    void readInput()
    {
        cin >> w;
        cin >> h;
        int c = _getchar_nolock();

        map = new Board * [h];
        for (int i = 0; i < h; i++)
        {
            map[i] = new Board[w];
            for (int j = 0; j < w; j++) {
                c = _getchar_nolock();
                map[i][j].field = (char)c;
                if (c == '*') addCity(i, j);
                //cin >> map[i][j];
            }
            c = _getchar_nolock();
        }
    }

    bool is_valid(int x, int y) const {
        return x >= 0 && x < h && y >= 0 && y < w;
    }
    //check where is city label
    void locateCityName(int row, int column, char* result)
    {
        int start = 0;

        if (column != w - 1 && isalnum(map[row][column + 1].field)) {
            readCityName(row, column + 1, result);
            return;
        }
        if (column != 0 && isalnum(map[row][column - 1].field)) {
            start = findNameStart(row, column - 1);
            readCityName(row, start, result);
            return;
        }
        if (column != 0 && row != 0 && isalnum(map[row - 1][column - 1].field) && !isalnum(map[row - 1][column].field)) {
            start = findNameStart(row - 1, column - 1);
            readCityName(row - 1, start, result);
            return;
        }
        if (column != 0 && row != h - 1 && isalnum(map[row + 1][column - 1].field) && !isalnum(map[row + 1][column].field)) {
            start = findNameStart(row + 1, column - 1);
            readCityName(row + 1, start, result);
            return;
        }
        if (column != w - 1 && row != 0 && isalnum(map[row - 1][column + 1].field) && !isalnum(map[row - 1][column].field)) {
            readCityName(row - 1, column + 1, result);
            return;
        }
        if (column != w - 1 && row != h - 1 && isalnum(map[row + 1][column + 1].field) && !isalnum(map[row + 1][column].field)) {
            readCityName(row + 1, column + 1, result);
            return;
        }
        if (row != 0 && isalnum(map[row - 1][column].field)) {
            if (column != 0 && !isalnum(map[row - 1][column - 1].field)) {
                start = column;
            }
            else if (column > 2 && !isalnum(map[row - 1][column - 2].field)) {
                start = column - 1;
            }
            else if (column < w - 1 && !isalnum(map[row - 1][column + 1].field)) {
                start = findNameStart(row - 1, column);
            }
            else if (column < w - 2 && !isalnum(map[row - 1][column + 2].field)) {
                start = findNameStart(row - 1, column + 1);
            }
            readCityName(row - 1, start, result);
            return;
        }
        if (row != h - 1 && isalnum(map[row + 1][column].field)) {
            if (column != 0 && !isalnum(map[row + 1][column - 1].field)) {
                start = column;
            }
            else if (column > 2 && !isalnum(map[row + 1][column - 2].field)) {
                start = column - 1;
            }
            else if (column < w - 1 && !isalnum(map[row + 1][column + 1].field)) {
                start = findNameStart(row + 1, column);
            }
            else if (column < w - 2 && !isalnum(map[row + 1][column + 2].field)) {
                start = findNameStart(row + 1, column + 1);
            }
            readCityName(row + 1, start, result);
            return;
        }
    }

    /*
    void logCities() {
        cout << "\nLOCATED CITIES\n";
        for (int i = 0; i < (int)cities.size(); i++)
        {
            cout << cities[i].name << ' ' << cities[i].row << ' ' << cities[i].column << endl;
        }
        cout << "\n";
    }
    void logFlights() {
        cout << "\nLOCATED FLIGHTS\n";
        for (int i = 0; i < (int)flights.size(); i++)
        {
            cout << flights[i].dep << ' ' << flights[i].dest << ' ' << flights[i].time << endl;
        }
        cout << "\n";
    }*/

    //searching label first letter
    int findNameStart(int row, int column) {
        if (column == -1) {
            return 0;
        }
        if (isalnum(map[row][column].field)) {
            return findNameStart(row, column - 1);
        }
        else {
            return column + 1;
        }
    }

    //read label
    void readCityName(int row, int column, char* name)
    {
        int i = 0;
        while (column < w && isalnum(map[row][column].field)) {
            name[i] = map[row][column].field;
            i++;
            column++;
        }
        name[i] = '\0';
    }

    void createGraph() {
        graph = new Vector<Path>[cities.size()];
    }
    void addCity(int i, int j)
    {
        City c = { NULL, i, j };
        cities.push_back(c);
    }

    void bfs(Point start, Vector<Point>* adj)
    {
        Queue queue;

        Vector<Point> vis;

        queue.push(start);
        map[start.x][start.y].visited = true;
        vis.push_back({ start.x, start.y });

        while (!queue.empty()) {
            Point curr = queue.pop();

            int dx[] = { -1, 0, 0, 1 };
            int dy[] = { 0, -1, 1, 0 };
            for (int i = 0; i < 4; i++) {
                int nx = curr.x + dx[i];
                int ny = curr.y + dy[i];

                if (is_valid(nx, ny) && (map[nx][ny].field == '*' || map[nx][ny].field == '#') && !map[nx][ny].visited)
                {
                    map[nx][ny].visited = true;
                    vis.push_back({ nx, ny });
                    Point p = { nx, ny };
                    if (map[nx][ny].field == '#')
                    {
                        p.d = curr.d + 1;
                        queue.push(p);
                    }
                    if (map[nx][ny].field == '*') {
                        p.d = curr.d + 1;
                        adj->push_back(p);
                    }
                }
            }
        }
        for (int i = 0; i < vis.size(); i++)
        {
            map[vis[i].x][vis[i].y].visited = false;
        }
    }

    /*
    void addFlights()
    {
        for (int i = 0; i < (int)flights.size(); i++)
        {
            int depIndex = mapNameCityIndex->get(flights[i].dep);
            int destIndex = mapNameCityIndex->get(flights[i].dest);
            
            bool found = false;
            for (int j = 0; j < graph[depIndex].size(); j++)
            {
                if (graph[depIndex][j].targetId == destIndex)
                {
                    if (graph[depIndex][j].time > flights[i].time)
                    {
                        graph[depIndex][j].time = flights[i].time;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) graph[depIndex].push_back({ destIndex, flights[i].time });
        }
    }*/
    void logGraph() {
        cout << "GRAPH\n";
        for (int i = 0; i < (int)cities.size(); i++)
        {
            cout << i << ". " << cities[i].name;
            for (int j = 0; j < (int)graph[i].size(); j++) {
                cout << " [" << graph[i][j].targetId << '-' << graph[i][j].time << "]";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    /// DIJKSTRA ///

    void searchRoute() {
        for (int i = 0; i < (int)requests.size(); i++)
        {
            int depIndex = mapNameCityIndex->get(requests[i].dep);
            int destIndex = mapNameCityIndex->get(requests[i].dest);
            searchRoute(depIndex, destIndex, requests[i].mode);
        }
    }

    void searchRoute(int depIndex, int destIndex, int mode)
    {
        int citiesNumber = (int)cities.size();

        PriorityQueue pq;
        int* dist = new int[citiesNumber];
        int* p = new int[citiesNumber];

        for (int i = 0; i < citiesNumber; i++) {
            dist[i] = INT_MAX;
            p[i] = -1;
        }

        pq.enqueue(Path(0, depIndex), 0);
        dist[depIndex] = 0;

        while (!pq.isEmpty())
        {
            int u = pq.dequeue().targetId;

            for (int i = 0; i < graph[u].size(); i++)
            {
                int v = graph[u][i].time;
                int weight = graph[u][i].targetId;
                if (dist[v] > dist[u] + weight)
                {
                    dist[v] = dist[u] + weight;
                    p[v] = u;
                    pq.enqueue(Path(dist[v], v), dist[v]);
                }
            }
        }
        cout << dist[destIndex];
        delete[] dist;

        if (mode == 1)
        {
            Vector<char*> tmp;
            for (int i = destIndex; i != depIndex; i = p[i])
            {
                tmp.push_back(cities[p[i]].name);
            }
            for (int jj = (int)tmp.size() - 2; jj >= 0; jj--)
            {
                cout << ' ' << tmp[jj];
            }
        }
        delete[] p;

        cout << endl;
    }
};

int main()
{
    Map* board = new Map();
    board->readInput();

    Vector<Point>* adj = new Vector<Point>[board->cities.size()];

    board->mapNameCityIndex = new CharPtrHashMap(board->cities.size());
    board->mapPointCityIndex = new IntPairHashMap(board->cities.size());

    for (int i = 0; i < board->cities.size(); i++)
    {
        char* cityName = new char[20];
        int x = board->cities[i].row;
        int y = board->cities[i].column;

        board->locateCityName(x, y, cityName);
        board->cities[i].name = cityName;
        board->mapNameCityIndex->put(cityName, i);
        board->mapPointCityIndex->put(Point(x, y), i);
        Vector<Point>* _adj = new Vector<Point>();
        board->bfs(Point(x, y), _adj);
        adj[i] = *_adj;
    }

    for (int i = 0; i < board->h; i++)
    {
        delete[] board->map[i];
    }
    delete[] board->map;

    board->createGraph();

    //board->logCities();
    board->readFlights();
    //board->logFlights();

    board->readRequests();

    for (int i = 0; i < (int)board->cities.size(); i++)
    {
        for (int j = 0; j < (int)adj[i].size(); j++) {
            board->graph[i].push_back({ board->mapPointCityIndex->get(Point(adj[i][j].x, adj[i][j].y)), adj[i][j].d });
        }
    }

    delete[] adj;

    //board->addFlights();
    //board->logGraph();
    board->searchRoute();
    for (int i = 0; i < (int)board->cities.size(); i++)
    {
        delete board->cities[i].name;
    }
    delete board->mapNameCityIndex;
    delete board->mapPointCityIndex;

    delete[] board->graph;
    delete board;
    return 0;
}
