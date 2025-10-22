#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

using namespace std;

class Player
{
public:
    Player(){}
    Player(string nm, int ng): name(nm), num_guesses(ng){}
    
    void SetName(string nm) { name = nm; }
    void SetNumGuesses(int ng) { num_guesses = ng; }
    
    string GetName() { return name; }
    int GetNumGuesses() { return num_guesses; }
    
private:
    string name;
    int num_guesses;
};

class LeaderBoard
{
public:
    LeaderBoard();
    void Readleaders(const char* filename);
    void WriteLeaders(const char* filename);
    void InsertPlayer(Player player);
    void DisplayLeaders();
    
private:
    static const int NUM_LEADERS = 5;
    Player leaders[NUM_LEADERS];
    int leader_count;
};

LeaderBoard::LeaderBoard()
{
    leader_count = 0;
}

void LeaderBoard::Readleaders(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
        return;
    
    leader_count = 0;
    char name_buffer[50];
    int guesses;
    
    while (leader_count < NUM_LEADERS && fscanf(file, "%s %d", name_buffer, &guesses) == 2)
    {
        leaders[leader_count] = Player(string(name_buffer), guesses);
        leader_count++;
    }
    
    fclose(file);
}

void LeaderBoard::WriteLeaders(const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
        return;
    
    for (int i = 0; i < leader_count; i++)
    {
        fprintf(file, "%s %d\n", leaders[i].GetName().c_str(), leaders[i].GetNumGuesses());
    }
    
    fclose(file);
}

void LeaderBoard::InsertPlayer(Player player)
{
    int insert_pos = leader_count;
    
    for (int i = 0; i < leader_count; i++)
    {
        if (player.GetNumGuesses() < leaders[i].GetNumGuesses())
        {
            insert_pos = i;
            break;
        }
    }
    
    if (insert_pos < NUM_LEADERS)
    {
        for (int i = (leader_count < 4 ? leader_count : 4); i > insert_pos; i--)
        {
            leaders[i] = leaders[i - 1];
        }
        
        leaders[insert_pos] = player;
        
        if (leader_count < NUM_LEADERS)
            leader_count++;
    }
}

void LeaderBoard::DisplayLeaders()
{
    cout << "Here are the current leaders:\n";
    for (int i = 0; i < leader_count; i++)
    {
        cout << leaders[i].GetName() << " made " << leaders[i].GetNumGuesses() << " guesses\n";
    }
}

void PlayGuessingGame(LeaderBoard& leaderboard)
{
    string name;
    cout << "Please enter your name to start: ";
    cin >> name;
    
    Player current_player;
    current_player.SetName(name);
    
    int target = (rand() % 91) + 10;
    double sqrt_value = sqrt((double)target);
    
    cout.precision(8);
    cout << fixed << sqrt_value << " is the square root of what number? ";
    
    int num_guesses = 0;
    int guess;
    
    while (1)
    {
        cout << "Guess a value between 10 and 100: ";
        cin >> guess;
        num_guesses++;
        
        if (guess == target)
        {
            cout << "You got it, baby!\n";
            break;
        }
        else if (guess > target)
        {
            cout << "Too high, guess again: ";
        }
        else
        {
            cout << "Too low, guess again: ";
        }
    }
    
    cout << "You made " << num_guesses << " guesses.\n";
    current_player.SetNumGuesses(num_guesses);
    
    leaderboard.InsertPlayer(current_player);
    leaderboard.DisplayLeaders();
}

int main()
{
    srand(time(NULL));
    
    LeaderBoard leaderboard;
    
    leaderboard.Readleaders("leaderboard.txt");
    
    cout << "Welcome! Press 'q' to quit or any other key to continue:\n";
    
    char c;
    bool game_over = false;
    
    while (!game_over)
    {
        cin >> c;
        
        if (c == 'q')
        {
            game_over = true;
            cout << "Bye Bye!\n";
        }
        else
        {
            PlayGuessingGame(leaderboard);
            cout << "Press 'q' to quit or any other key to continue:\n";
        }
    }
    
    leaderboard.WriteLeaders("leaderboard.txt");
    
    return 0;
}