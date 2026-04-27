#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <cctype>
#include <cstdlib>
#include <unordered_set>
#include <iomanip>
#include <climits>

using namespace std;

// ensure safe input for integers within a range
int safeInput(int min, int max, const string& prompt) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= min && choice <= max)
            return choice;
        cout << "❌ Invalid input. Please enter " << min << "-" << max << ".\n";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

// clear console screen
void clearScreen() {
    system("clear");  
}

// stores book information
struct Book {
    string title;
    string isbn;
    string author;
    string genre;
    int year;
    double rating;
};

// graph structure to store books and their relationships
class Graph {
public:
    vector<Book> books;
    vector<vector<int>> adj;
    vector<bool> edgesBuilt;

    void addBook(Book b) {
        int old_n = books.size();
        books.push_back(b);
        edgesBuilt.push_back(false);

        for (int i = 0; i < old_n; i++) {
            adj[i].resize(books.size(), 0);
        }

        adj.emplace_back(books.size(), 0);
    }

    void addEdge(int a, int b, int weight) {
        if (a >= 0 && a < books.size() && b >= 0 && b < books.size()) {
            adj[a][b] = weight;
            adj[b][a] = weight;
        }
    }

    int getWeight(const Book &a, const Book &b) {
        int weight = 0;

        if (a.genre == b.genre) weight += 3;
        if (a.author == b.author) weight += 3;
        if (abs(a.year - b.year) <= 2) weight += 2;

        return weight;
    }

    void buildEdgesFrom(int startIdx) {
        if (edgesBuilt[startIdx]) return;

        for (int i = 0; i < books.size(); i++) {
            int weight = getWeight(books[startIdx], books[i]);

            if (weight > 0) {
                addEdge(startIdx, i, weight);
            }
        }
        edgesBuilt[startIdx] = true;
    }
};

// genre-based tree for accessing books by genre
struct GenreNode {
    string genre;
    vector<int> indexes;
};

class GenreTree {
public:
    vector<GenreNode> tree;

    void addBook(string genre, int idx) {
        for (auto &n : tree) {
            if (n.genre == genre) {
                n.indexes.push_back(idx);
                return;
            }
        }
        tree.push_back({genre, {idx}});
    }

    vector<int> get(string genre) {
        for (auto &n : tree)
            if (n.genre == genre)
                return n.indexes;
        return {};
    }
};

// case-insensitive search function to find books by title or author
string toLowerStr(string s) {
    for (char &c : s)
        c = tolower(c);
    return s;
}

vector<int> linearSearch(Graph &g, string key) {
    vector<int> res;
    string k = toLowerStr(key);

    for (int i = 0; i < g.books.size(); i++) {
        if (toLowerStr(g.books[i].title).find(k) != string::npos ||
            toLowerStr(g.books[i].author).find(k) != string::npos) {
            res.push_back(i);
        }
    }

    return res;
}

// sorting algorithms for recommendations
int partition(vector<Book> &arr, int low, int high) {
    double pivot = arr[high].rating;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].rating > pivot) {
        i++;
        swap(arr[i], arr[j]);
        }
    }
        swap(arr[i + 1], arr[high]);
        return i + 1;
        }

void quickSort(vector<Book> &arr, int low, int high) {
    if (low < high) {
    int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
        }
    }

void merge(vector<Book> &arr, int l, int m, int r) {
    vector<Book> L(arr.begin() + l, arr.begin() + m + 1);
    vector<Book> R(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;

    while (i < L.size() && j < R.size()) {
        if (L[i].year > R[j].year)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < L.size()) arr[k++] = L[i++];
    while (j < R.size()) arr[k++] = R[j++];
}

void mergeSort(vector<Book> &arr, int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// main library class that manages books, search, recommendations, and user interactions
class Library {
private:
    Graph g;
    GenreTree tree;
    queue<int> recent;
    vector<int> favorites;
    unordered_set<int> favorites_set; // fast lookup for favorites

public:
    void addBook(string t, string isbn, string a, string ge, int y, double r) {
        g.addBook({t, isbn, a, ge, y, r});
        tree.addBook(ge, g.books.size() - 1);
    }

    vector<Book> getTopBooks(vector<pair<double, int>> &scored, int limit) {
    vector<Book> topBooks;

    for (int i = 0; i < limit; i++) {
        topBooks.push_back(g.books[scored[i].second]);
    }

    return topBooks;
    }

  double computeScore(int i) {
    double score = g.books[i].rating * 3.0; // rating influence

    if (favorites_set.count(i)) score += 15.0; // direct favorite boost
    {
        queue<int> temp = recent;
        int pos = 0;
        while (!temp.empty()) {
            if (temp.front() == i)
                score += max(1.0, 1.0 - pos * 0.8); // recent boost with decay
            temp.pop();
            pos++;
        }
    }

    for (int f : favorites) {
        int w = g.getWeight(g.books[f], g.books[i]);
        if (w > 0)
            score += w * 2;  // graph boost for favorites similarity
    }

    {
        queue<int> temp = recent;
        while (!temp.empty()) {
            int w = g.getWeight(g.books[temp.front()], g.books[i]);
            if (w > 0)
                score += w;  // graph boost for recent similarity
            temp.pop();
        }
    }

    score += (g.books[i].year - 2000) * 0.05; // small boost for newer books
    return score;
}

    // view book details and allow adding to favorites
    void view(int idx) {
        if (idx < 0 || idx >= g.books.size()) {
            cout << "❌ Invalid index.\n";
            return;
        }

        Book b = g.books[idx];

        cout << "\n=============== 📖 BOOK DETAILS ===============\n";
        cout << "Title : " << b.title << "\n";
        cout << "ISBN  : " << b.isbn << "\n";
        cout << "Author: " << b.author << "\n";
        cout << "Genre : " << b.genre << "\n";
        cout << "Year  : " << b.year << "\n";
        cout << "Rating: " << b.rating << "\n";

        recent.push(idx);
        if (recent.size() > 5) recent.pop();

        char fav;
        cout << "\n⭐ Add to favorites? (y/n): ";
        cin >> fav;
        cin.ignore();

        if (fav == 'y' || fav == 'Y') {
            if (favorites_set.find(idx) == favorites_set.end()) {
                favorites.push_back(idx);
                favorites_set.insert(idx);
                cout << "⭐ Added to favorites!\n";
            } else {
                cout << "⭐ Already in favorites!\n";
            }
        }
    }

    // search for books by title or author and allow exploration of related books
    void search(string key) {
        auto res = linearSearch(g, key);

        cout << "\n🔍 RESULTS:\n";
        if (res.empty()) {
            cout << "No books found.\n";
            return;
        }

        for (int i = 0; i < res.size(); i++)
            cout << i << " - " << g.books[res[i]].title << "\n";

        int pick;
        cout << "\nSelect index (-1 cancel): ";
        cin >> pick;
        cin.ignore();

        if (pick < 0 || pick >= res.size()) return;
        int idx = res[pick];

        while (true) {
            view(idx);
            // explore more books
            int choice;

        while (true) {
        cout << "\t\t\t\t========================================\n";                          
        cout << "\t\t\t\t▄▄▄▄▄▄▄             ▄▄\n";                  
        cout << "\t\t\t\t███▀▀▀▀▀             ██\n";                  
        cout << "\t\t\t\t███▄▄    ██ ██ ████▄ ██ ▄███▄ ████▄ ▄█▀█▄\n";
        cout << "\t\t\t\t███       ███  ██ ██ ██ ██ ██ ██ ▀▀ ██▄█▀\n";
        cout << "\t\t\t\t▀███████ ██ ██ ████▀ ██ ▀███▀ ██    ▀█▄▄▄\n";
        cout << "\t\t\t\t               ██                         \n";
        cout << "\t\t\t\t               ▀▀                         \n";
        cout << "\t\t\t\t==========================================\n\n";

                cout << "1. 🌳 Same Genre Books\n";
                cout << "2. 🌐 Related Books\n";
                cout << "3. 🔄 Back to Main Menu\n";
                cout << "Choice: ";
                cin >> choice;
                cin.ignore();

                Book b = g.books[idx];

                if (choice == 1) {
                clearScreen();
                    cout << "\n🌳 SAME GENRE:\n";

                    vector<int> genreBooks = tree.get(b.genre);
                    vector<int> options;

                    for (int i : genreBooks) {
                        if (i != idx) {
                            cout << options.size() << " - " << g.books[i].title << "\n";
                            options.push_back(i);
                        }
                    }

                    if (options.empty()) {
                        cout << "No same genre books.\n";
                        continue;
                    }

                    int pick2 = safeInput(-1, options.size() - 1, "Select book (-1 cancel): ");
                    if (pick2 < 0 || pick2 >= options.size()) continue;

                    idx = options[pick2];
                    break;
                }

                // related books using Dijkstra's algorithm on the graph
                else if (choice == 2) {
                clearScreen();
                    cout << "\n🌐 RELATED BOOKS:\n";

                    vector<int> dist(g.books.size(), INT_MAX);
                    vector<bool> visited(g.books.size(), false);
                    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
                    vector<int> options;

                    dist[idx] = 0;
                    pq.push({0, idx});

                    int count = 0;
                    const int LIMIT = 5;

                    while (!pq.empty() && count < LIMIT) {
                        auto [d, node] = pq.top();
                        pq.pop();

                        if (visited[node]) continue;
                        visited[node] = true;

                        if (node != idx) {
                            cout << options.size() << " - " << g.books[node].title << "\n";
                            options.push_back(node);
                            count++;
                        }

                        g.buildEdgesFrom(node);

                        for (int i = 0; i < g.books.size(); i++) {
                            if (g.adj[node][i] > 0 && !visited[i]) {
                                int newDist = d + (100 - g.adj[node][i] * 10);  // Convert weight to distance
                                if (newDist < dist[i]) {
                                    dist[i] = newDist;
                                    pq.push({newDist, i});
                                }
                            }
                        }
                    }

                    if (options.empty()) {
                        cout << "No related books found.\n";
                        continue;
                    }

                    int pick2 = safeInput(-1, options.size() - 1, "Select book (-1 cancel): ");
                    if (pick2 < 0 || pick2 >= options.size()) continue;

                    idx = options[pick2];
                    break;  
                }

                else if (choice == 3) return;
            }
        }
    }

    // view books in favorites or recently viewed
    void viewBookMenu() {
        while (true) {
            cout << "1. 💝 Favorite Books (" << favorites.size() << ")\n";
            cout << "2. 📚 Recently Viewed (" << recent.size() << ")\n";
            cout << "3. 🔄 Back\n";
            cout << "Choice: ";
            int choice = safeInput(1, 3, "");

            if (choice == 3) return;

            vector<int> books_to_show;
            string label;

            if (choice == 1) {
                if (favorites.empty()) {
                    cout << "❌ No favorite books yet!\n";
                    cout << "\n==============================\n";
                    continue;
                }
                books_to_show = favorites;
                label = "💝 FAVORITES";
            } else {
                if (recent.empty()) {
                    cout << "❌ No recently viewed books yet!\n";
                    cout << "\n==============================\n";
                    continue;
                }

                queue<int> temp = recent; // to preserve original queue
                vector<int> recentList;
                while (!temp.empty()) {
                    recentList.push_back(temp.front());
                    temp.pop();
                }
                reverse(recentList.begin(), recentList.end());
                books_to_show = recentList;
                label = "📚 RECENTLY VIEWED";
            }
           
            clearScreen();
            cout << "\n" << label << ":\n";
            cout << "====================================\n";
            for (int i = 0; i < books_to_show.size(); i++) {
                int idx = books_to_show[i];
                cout << i << " - " << g.books[idx].title << " by " << g.books[idx].author << "\n";
            }

            int pick = safeInput(-1, books_to_show.size() - 1, "Select book (-1 cancel): ");
            if (pick < 0 || pick >= books_to_show.size()) continue;

            int idx = books_to_show[pick];

            while (true) {
                view(idx);

                int option;
                while (true) {
        cout << "\t\t\t\t========================================\n";                          
        cout << "\t\t\t\t▄▄▄▄▄▄▄             ▄▄\n";                  
        cout << "\t\t\t\t███▀▀▀▀▀             ██\n";                  
        cout << "\t\t\t\t███▄▄    ██ ██ ████▄ ██ ▄███▄ ████▄ ▄█▀█▄\n";
        cout << "\t\t\t\t███       ███  ██ ██ ██ ██ ██ ██ ▀▀ ██▄█▀\n";
        cout << "\t\t\t\t▀███████ ██ ██ ████▀ ██ ▀███▀ ██    ▀█▄▄▄\n";
        cout << "\t\t\t\t               ██                         \n";
        cout << "\t\t\t\t               ▀▀                         \n";
        cout << "\t\t\t\t==========================================\n\n";

                    cout << "1. 🌳 Same Genre Books\n";
                    cout << "2. 🌐 Related Books\n";
                    cout << "3. 🔄 Back\n";
                    cout << "Choice: ";
                    cin >> option;
                    cin.ignore();
                    clearScreen();

                    Book b = g.books[idx];

                    if (option == 1) {
                        cout << "\n🌳 SAME GENRE:\n";

                        vector<int> genreBooks = tree.get(b.genre);
                        vector<int> options_list;

                        for (int i : genreBooks) {
                            if (i != idx) {
                                cout << options_list.size() << " - " << g.books[i].title << "\n";
                                options_list.push_back(i);
                            }
                        }

                        if (options_list.empty()) {
                            cout << "No same genre books.\n";
                            continue;
                        }

                        int pick2 = safeInput(0, options_list.size() - 1, "Select book (-1 cancel): ");                       
                        if (pick2 < 0 || pick2 >= options_list.size()) continue;

                        idx = options_list[pick2];
                        break;
                    }

                    else if (option == 2) {
                        cout << "\n🌐 RELATED BOOKS:\n";

                        vector<int> dist(g.books.size(), INT_MAX);
                        vector<bool> visited(g.books.size(), false);
                        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
                        vector<int> options_list;

                        dist[idx] = 0;
                        pq.push({0, idx});

                        int count = 0;
                        const int LIMIT = 5;

                        while (!pq.empty() && count < LIMIT) {
                            auto [d, node] = pq.top();
                            pq.pop();

                            if (visited[node]) continue;
                            visited[node] = true;

                            if (node != idx) {
                                cout << options_list.size() << " - " << g.books[node].title << "\n";
                                options_list.push_back(node);
                                count++;
                            }

                            g.buildEdgesFrom(node);

                            for (int i = 0; i < g.books.size(); i++) {
                                if (g.adj[node][i] > 0 && !visited[i]) {
                                    int newDist = d + (100 - g.adj[node][i] * 10);  // Convert weight to distance
                                    if (newDist < dist[i]) {
                                        dist[i] = newDist;
                                        pq.push({newDist, i});
                                    }
                                }
                            }
                        }

                        if (options_list.empty()) {
                            cout << "No related books found.\n";
                            continue;
                        }

                        int pick2 = safeInput(0, options_list.size() - 1, "Select book (-1 cancel): ");
                        if (pick2 < 0 || pick2 >= options_list.size()) continue;

                        idx = options_list[pick2];
                        break;
                    }

                    else if (option == 3) return;
                }
            }
        }
    }


// compute personalized score for recommendations based on rating, favorites, recency, and graph similarity  
void recommendations() {

    if (favorites.empty() && recent.empty()) {
        cout << "\n❌ Need more data! View some books or add favorites first.\n";
        cout << "\n 🔄 Back to main menu (press 'y'): ";
        char choice;
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            return;
        }
    }

    if (g.books.empty()) {
        cout << "📚 No books in library yet!\n";
        return;
    }

    vector<pair<double, int>> scored;

    for (int i = 0; i < g.books.size(); i++) {
        scored.push_back({computeScore(i), i});
    }

    sort(scored.rbegin(), scored.rend());

    // extract top books
    int showCount = min(5, (int)scored.size());
    vector<Book> recBooks;

    for (int i = 0; i < showCount; i++) {
        recBooks.push_back(g.books[scored[i].second]);
    }

    cout << "Sort recommendations by:\n";
    cout << "1. 🎯 Personalized Score\n";
    cout << "2. ⭐ Highest Rating \n";
    cout << "3. 🕒 Most Recent Year\n";
    cout << "Choice: ";

    int sortChoice = safeInput(1, 3, "");

    // sort by rating or year if user chooses
    if (sortChoice == 2) {
        quickSort(recBooks, 0, recBooks.size() - 1);
    }
    else if (sortChoice == 3) {
        mergeSort(recBooks, 0, recBooks.size() - 1);
    }

    clearScreen();
    cout << "\n📚 FINAL RECOMMENDATIONS:\n";
    cout << "\n===============================\n";

    for (int i = 0; i < recBooks.size(); i++) {
        cout << i << " - "
             << recBooks[i].title
             << " | ⭐ " << fixed << setprecision(1) << recBooks[i].rating
             << " | 📅 " << recBooks[i].year
             << "\n";
    }

    int pick = safeInput(-1, recBooks.size() - 1, "Select book to view (-1 cancel): ");
    if (pick < 0) return;

    for (int i = 0; i < g.books.size(); i++) {
        if (g.books[i].title == recBooks[pick].title &&
            g.books[i].author == recBooks[pick].author) {

            view(i);
            break;
        }
    }
}


    // main menu for user interaction
    void menu() {
        int c;

        while (true) {
        clearScreen();
cout << "\t\t\t========================================================================\n";                                                                                          
cout << "\t\t\t▄▄▄▄▄▄▄                      ▄▄▄      ▄▄▄                             ▄▄\n";
cout << "\t\t\t███▀▀███▄             ▄▄     ████▄  ▄████             ▄▄              ██\n";
cout << "\t\t\t███▄▄███▀ ▄███▄ ▄███▄ ██ ▄█▀ ███▀████▀███  ▀▀█▄ ████▄ ██ ▄█▀ ▄█▀█▄ ▄████\n";
cout << "\t\t\t███  ███▄ ██ ██ ██ ██ ████   ███  ▀▀  ███ ▄█▀██ ██ ▀▀ ████   ██▄█▀ ██ ██\n";
cout << "\t\t\t████████▀ ▀███▀ ▀███▀ ██ ▀█▄ ███      ███ ▀█▄██ ██    ██ ▀█▄ ▀█▄▄▄ ▀████\n";
cout << "\t\t\t========================================================================\n\n";      
                                               
            cout << "1. 🔍 Search Book\n\n";
            cout << "2. 📖 View Books\n\n";
            cout << "3. 🎯 Personalized Recommendation\n\n";
            cout << "4. 🚪 Exit\n";
            cout << "Choice: ";
            cin >> c;
            cin.ignore();

            if (c == 1) {                                                
            clearScreen();  
cout << "\t\t\t\t=======================================\n";                                        
cout << "\t\t\t\t ▄▄▄▄▄▄▄                         ▄▄      \n";
cout << "\t\t\t\t █████▀▀▀                         ██     \n";  
cout << "\t\t\t\t  ▀████▄  ▄█▀█▄  ▀▀█▄ ████▄ ▄████ ████▄    \n";
cout << "\t\t\t\t    ▀████ ██▄█▀ ▄█▀██ ██ ▀▀ ██    ██ ██   \n";
cout << "\t\t\t\t ███████▀ ▀█▄▄▄ ▀█▄██ ██    ▀████ ██ ██   \n";
cout << "\t\t\t\t=======================================\n\n";  
                                                   
                string k;
                cout << "Keyword: ";
                getline(cin, k);
                search(k);
            } else if (c == 2) {
                clearScreen();
cout << "\t\t\t\t\t=================================\n";                            
cout << "\t\t\t\t\t ▄▄▄▄  ▄▄▄▄                        \n";
cout << "\t\t\t\t\t ▀███  ███▀ ▀▀               \n";
cout << "\t\t\t\t\t  ███  ███  ██  ▄█▀█▄ ██   ███▄  \n";
cout << "\t\t\t\t\t  ███▄▄███  ██  ██▄█▀ ██ █ ██\n";
cout << "\t\t\t\t\t   ▀████▀   ██▄ ▀█▄▄▄  ██▀██\n";
cout << "\t\t\t\t\t=================================\n\n";
                viewBookMenu();
            } else if (c == 3) {
                clearScreen();  
                       
cout << "\t\t\t\t=======================================================\n";                                                                                  
cout << "\t\t\t\t▄▄▄▄▄▄▄                                              ▄▄ \n";
cout << "\t\t\t\t█████▀▀▀                                ██            ██ \n";
cout << "\t\t\t\t▀████▄  ██ ██ ▄████ ▄████ ▄█▀█▄ ▄█▀▀▀ ▀██▀▀ ▄█▀█▄ ▄████ \n";
cout << "\t\t\t\t  ▀████ ██ ██ ██ ██ ██ ██ ██▄█▀ ▀███▄  ██   ██▄█▀ ██ ██ \n";
cout << "\t\t\t\t███████▀ ▀██▀█ ▀████ ▀████ ▀█▄▄▄ ▄▄▄█▀  ██   ▀█▄▄▄ ▀████ \n";
cout << "\t\t\t\t                  ██    ██                               \n";
cout << "\t\t\t\t                ▀▀▀   ▀▀▀                                \n";  
cout << "\t\t\t\t=======================================================\n";                      
               
            recommendations();  
            } else if (c == 4) {
                cout << "Thank you for using BookMarked! 👋\n";
                break;
            }
        }
    }

};


int main() {
    Library lib;

    lib.addBook("Clean Code", "1", "Robert C. Martin", "Software", 2008, 4.8);
    lib.addBook("Secure Coding", "2", "Pavel Yosifovich", "Software", 2025, 4.9);
    lib.addBook("Operating System Concepts", "3", "Silberschatz", "Systems", 2011, 4.0);
    lib.addBook("Data Structures & Algorithms", "4", "Mark Allen Weiss", "CS", 2007, 4.6);
    lib.addBook("Design Patterns", "5", "Erich Gamma", "Software", 2010, 4.7);
    lib.addBook("Modern C++", "6", "Andrei Alexandrescu", "Software", 2023, 4.9);
    lib.addBook("Introduction to Microprocessors", "0-7506-3787-0", "John Crisp", "Engineering", 2001, 3.8);
    lib.addBook("Introduction to Physics (10th Edition)", "978-1-11992-247-6", "John Cutnell", "Physics", 2015, 3.2);
    lib.addBook("Chemistry: The Molecular Nature of Matter", "978-0-07-107855-9", "Martin Silberberg", "Chemistry", 2008, 3.7);
    lib.addBook("Chemistry for Engineering Students", "978-981-4392-99-0", "Lawrence Brown | Tom Holme", "Chemistry", 2012, 4.3);
    lib.addBook("Kirkpatrick’s Physics: A World View", "0-495-11006-X", "Larry Kirkpatrick | Gerald Wheeler", "Physics", 2006, 5.0);
    lib.addBook("Trigonometry (4th Edition)", "0-13-152726-6", "Michael Sullivan", "Mathematics", 2005, 4.4);
    lib.addBook("Applied Numerical Methods for Engineers", "9971-51-139-8", "Terrence Akai", "Engineering", 1994, 4.0);
    lib.addBook("Engineering Design Communication", "0-201-33151-9", "Shawna Lockhart | Cindy Johnson", "Engineering", 1998, 4.3);
    lib.addBook("Robotics", "978-1-934015-02-5", "Appin Knowledge Solutions", "Robotics", 2008, 3.2);
    lib.addBook("Introduction to Robotics (2nd Edition)", "978-0-470-60446-5", "Saeed Niku", "Robotics", 2010, 4.1);
    lib.addBook("Introduction to Electric Circuits", "978-0-470-55302-2", "Richard Dorf | James A. Svoboda", "Engineering", 2010, 4.0);
    lib.addBook("Computer-Aided Power Systems Analysis", "978-1-4200-6106-2", "George Kusic", "Engineering", 2008, 3.8);
    lib.addBook("Engineering Circuit Analysis (10th Edition)", "978-0-470-87377-9", "David Irwin | Robert Nelms", "Engineering", 2011, 4.2);
    lib.addBook("Digital Signal Processing with Kernel Methods", "978-1-118-61179-1", "Jose Luis Rojo-Alvarez et al.", "Engineering", 2018, 3.5);
    lib.addBook("Secure Coding", "978-1-394-17170-5", "Tanya Janca", "Computer Science", 2025, 4.5);
    lib.addBook("Database Systems (13th Edition)", "978-1-337-62790-0", "Carlos Coronel | Steven Morris", "Computer Science", 2018, 4.4);
    lib.addBook("Data Structures and Algorithms for Game Dev", "978-1-58450-495-5", "Allen Sherrod", "Computer Science", 2007, 4.6);
    lib.addBook("Operating Systems Concepts (8th Edition)", "978-0-470-23399-3", "Abraham Silberschatz et al.", "Computer Science", 2011, 3.9);
    lib.addBook("Fundamentals of Differential Equations", "0-8053-5056-X", "Kent Nagle | Edward Saff", "Mathematics", 1993, 3.9);
    lib.addBook("Integral Calculus", "978-971-9900-10-8", "Thelma Montero-Galliguez et al.", "Mathematics", 2011, 3.5);
    lib.addBook("Differential Calculus", "978-1-25-906267-4", "Pratiksha Saxena", "Mathematics", 2014, 4.5);
    lib.addBook("Numerical Methods (2nd Edition)", "978-0-470-87374-8", "Amos Gilat | Vish Subramaniam", "Mathematics", 2011, 4.8);
    lib.addBook("Systems Analysis and Design", "978-981-4232-3", "Garry Shelly et al.", "Computer Science", 2007, 4.0);
    lib.addBook("Database Management Systems", "0-07-115508-2", "Raghu Ramakrishnan", "Computer Science", 1998, 4.2);
    lib.addBook("Computer Architecture: A Quantitative Approach", "978-0-12-383872-8", "John Hennessy | David Patterson", "Computer Science", 2011, 4.1);
    lib.addBook("Discrete Mathematics and Its Applications", "978-0-12-383872-8", "John Hennessy | David Patterson", "Mathematics", 2011, 4.1);
    lib.addBook("Discrete Mathematics with Combinatorics", "0-13-086998-8", "James Anderson", "Mathematics", 2000, 3.0);
    lib.addBook("Principles and Practices of Computer Programming", "978-93-81991-97-8", "Tanvir Ahmad", "Computer Science", 2014, 3.5);
    lib.addBook("Object-Oriented Programming in C# 5.0", "978-1-285-85456-4", "Bintu Harwani", "Computer Science", 2015, 3.8);
    lib.addBook("C++ Programming (6th Edition)", "978-1-133-52635-3", "Davender Malik", "Computer Science", 2012, 4.0);
    lib.addBook("Operating Systems (2nd Edition)", "978-1-133-52635-3", "Atul Prakash", "Computer Science", 2016, 3.5);
    lib.addBook("Intro to Embedded Systems (2nd Edition)", "978-0-262-53381-2", "Edward Lee | Sanjit Seshia", "Engineering", 2017, 4.5);
    lib.addBook("Embedded Systems Handbook", "978-1-68094-114-2", "Darren Samuels", "Engineering", 2015, 3.6);
    lib.addBook("Data Structures and Algorithms using Python", "978-0-470-61829-5", "Rance Necaise", "Computer Science", 2010, 3.9);
    lib.addBook("Teach Yourself MySQL in 21 Days", "0-672-31914-4", "Mark Maslakowski", "Computer Science", 2000, 3.0);
    lib.addBook("The AutoCAD LT 97 Tutor", "0-7668-0474-7", "Alan Kalameja", "Engineering", 1998, 3.0);
    lib.addBook("System Software (2nd Edition)", "0-201-50945-8", "Lelland Beck", "Computer Science", 1990, 3.9);
    lib.addBook("Software Design for Six-Sigma", "978-0-470-40546-8", "Basem El-Haik | Adnan Shaout", "Engineering", 2010, 4.0);
    lib.addBook("Software Architect", "978-1-119-82097-0", "Michael Bell", "Computer Science", 2023, 4.7);
    lib.addBook("AI Programming with Python", "978-1-119-82086-4", "Perry Xiao", "Computer Science", 2022, 3.8);
    lib.addBook("Digital Signal Processing", "0-07-027389-8", "Monson Hayes", "Engineering", 1999, 4.2);
    lib.addBook("Digital Systems Design", "978-93-5466-768-8", "Nagoor Kani", "Engineering", 2024, 4.0);
    lib.addBook("Modern Robotics: Mechanics, Planning, and Control", "978-1-107-15630-2", "Frank Park | Kevin Lynch", "Robotics", 2017, 4.5);
    lib.addBook("Industrial Robotics Fundamentals", "978-1-649-25978-3", "Larry Ross", "Robotics", 2018, 4.6);
    lib.addBook("Modern Thermodynamics", "9780471973942", "Dilip Kondepudi", "Physics", 1998, 4.3);
    lib.addBook("Conceptual Physics", "9780321568090", "Paul Hewitt", "Physics", 2009, 4.5);

    lib.menu();
}
