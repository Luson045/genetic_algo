#include <bits/stdc++.h>
#include <fstream>
using namespace std;

class Player{
  public:
  string name;
  vector<float> metrices;
  void update_stats(int idx,float new_stat){
    metrices[idx]=new_stat;
  }
  Player(string n, vector<float> matrix){
    name=n;metrices=matrix;
  }
  vector<Player> Cross(Player &p2){
    vector<float> temp = {0,0,0,0,0,0};
    //vector<float> temp2= {0,0,0,0,0,0};
    //since we are doing all combination so there will be another two similar set, so we remove one here
    float max_this=metrices[0];
    int max_index_this=0;//finding the maximum stat for the given class
    for(int i=0;i<metrices.size();i++){
      if(metrices[i]>max_this){
        max_this = metrices[i];
        max_index_this=i;
      }
    }
    float max_other=p2.metrices[0];
    int max_index_other=0;//finding the maximum stat for the argument class
    for(int i=0;i<p2.metrices.size();i++){
      if(p2.metrices[i]>max_other){
        max_other = p2.metrices[i];
        max_index_other=i;
      }
    }
    //interchanging the max stats; max stat of player 1 is applied to that particular stat of player2 and vice versa
    for(int i=0;i<metrices.size();i++){
      if(i==max_index_other){
        temp[i]=max_other;
      }else{
        temp[i]=metrices[i];
      }
    }
    //creating the new player with this stats, and for fun lets add both the name here with the parent with higher no. of stats first, example if messi's best stat is teamwork then it is applied in ronaldo's stat, then the name is ronaldo&messi
    string new_name=name+p2.name;
    Player new_player1(new_name,temp);

    /*for(int i=0;i<p2.metrices.size();i++){
      if(i==max_index_this){
        temp2[i]=max_this;
      }else{
        temp2[i]=p2.metrices[i];
      }
    }
    new_name=p2.name+name;
    Player new_player2(new_name,temp2);
    */
    vector<Player> offsprings={new_player1};
    return offsprings;
  }

  //utility function to see if there is a 90+ stat in each player
  bool isfit(){
    for(auto i:metrices){
      if(i>=90) return true;
    }
    return false;
  }

  void Mutate(){//add a random no. to any stat
    srand(time(0));
    int rand_luck = (rand()%10)%5;
    if(rand_luck==0){
      int random_stat=rand()%7;
      int random_index=rand()%6;
      cout<<metrices[random_index]<<"->";
      metrices[random_index]+=random_stat;
      cout<<metrices[random_index]<<endl;
    }
  }
};

//utility function to check how many players exists
int extinct(vector<Player> players){
  return players.size();
}

vector<Player> Fitness(vector<Player> allplayer){//f(x) x has a 90+ stat
  vector<Player> fitplayers;
  for(auto player:allplayer){
    if(player.isfit()){
      fitplayers.push_back(player);
    }
  }
  return fitplayers;
}

vector<Player> MultiCross(vector<Player> allplayer) {
    vector<Player> newplayers;
    for (auto player1 : allplayer) {
        for (auto player2 : allplayer) {
            if (player1.name != player2.name) {
                vector<Player> temp = player1.Cross(player2);
                newplayers.insert(newplayers.end(), temp.begin(), temp.end());
            }
        }
    }
    allplayer.insert(allplayer.end(), newplayers.begin(), newplayers.end());
    return allplayer;
}

vector<Player> genetic(vector<Player> players){
      // Display the created players (for verification)
      cout<<"INITIAL POPULATION: "<<extinct(players)<<endl;
      //step 1 selection of fit ones
      vector<Player> fitplayers=Fitness(players);
      cout<<"FIT POPULATION: "<<extinct(fitplayers)<<endl;

      //step 2 crossovers;
      players= MultiCross(fitplayers);
      cout<<"NEW POPULATION: "<<extinct(players)<<endl;


      //step3 mutation;
      for(auto player:players){
        player.Mutate();
      }
      cout<<"MUTATED"<<endl;
      return players;
}


int main() {
    ifstream data("data.txt");
    if (!data.is_open()) {
        cerr << "Error: Unable to open file!" << endl;
        return 1;
    }

    string line;
    getline(data, line);
    cout << line << endl;
    getline(data, line);
    cout << line << endl;

    vector<Player> players;
    while (getline(data, line)) {
        stringstream ss(line);
        string name;
        vector<float> metrices;
        string temp;

        // Extract name (everything before the first comma)
        getline(ss, name, ',');

        // Extract the stats (comma-separated values)
        while (getline(ss, temp, ',')) {
            metrices.push_back(stof(temp));
        }

        players.emplace_back(name, metrices);
    }

    data.close();
    for(int i=0;i<2;i++){
      players=genetic(players);
    }

    //finding the fittest player
    int max_stats=INT_MIN;
    int idx=0;
    int count=0;
    for(auto player:players){
      if(accumulate(player.metrices.begin(),player.metrices.end(),0.0f)>max_stats){
        max_stats=accumulate(player.metrices.begin(),player.metrices.end(),0.0f);
        idx=count;
      }
      count++;
    }
    Player best=players[idx];
    cout<<"and the new best player is: \n";
    cout<<"Name: "<<best.name<<endl;
    cout<<"Stats: ";
    for(auto s:best.metrices){
      cout<<s<<",";
    }
    cout<<endl;

    for(auto player:players){
      if(accumulate(player.metrices.begin(),player.metrices.end(),0.0f)>=580)
        cout<<"name:"<<player.name<<" points="<<accumulate(player.metrices.begin(),player.metrices.end(),0.0f)<<endl;
    }
    return 0;
}
