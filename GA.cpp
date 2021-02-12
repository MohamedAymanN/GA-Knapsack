#include<bits/stdc++.h>
#define pb push_back
#define adminMode 1 /** 1 for the proposed knapsack problem || 0 for user mode**/
using namespace std;
/** Car Parts Store Example
Name        Weight(kg)  Value(Egp)
Tire           7         800
BrakeRotor     9.5       200
AirBag         1         630
ArmRest        0.35      126
Battery        22        576
CrankShaft     31        900
Clock          1         216
Clutch         5         198
**/

struct object{
    string name;
    int weight;
    int price;
};

void mutate(string &child1,string &child2){
    int ch1Gene = rand() % child1.size();
    int ch2Gene = rand() % child2.size();
    if((rand() % 10) == 0) child1[ch1Gene] = (child1[ch1Gene]=='1' ? '0' : '1');
    if((rand() % 10) == 0) child2[ch2Gene] = (child2[ch2Gene]=='1' ? '0' : '1');
}

int calcFit(string chrom,vector<object*> objVec,int maxWeight){
    int totWeight=0, totPrice=0;
    for(int i=0; i < chrom.size(); i++){
        if(chrom[i]=='1'){
            totWeight += objVec[i]->weight;
            totPrice  += objVec[i]->price;
        }
    }
    return (totWeight>maxWeight ? 0 : totPrice);
}

void generatePopulation(vector<pair<string,int>>& population,int populationSize,int chromSize,vector<object*> &objVec,int maxWeight){
    int diff = populationSize - population.size();
    string choice ="01";
    while(diff--){
        string chrom(chromSize,'x');
        for(int i=0;i<chromSize;i++){
            chrom[i] = choice[rand()%2];
        }
        int fitNum = calcFit(chrom,objVec,maxWeight);
        population.pb({chrom,fitNum});
    }
}

void knapSack(vector<object*> &objVec,int populationSize,int numIterations,int maxWeight){
    int chromSize = objVec.size();
    string chromosome(chromSize,'0');
    vector<pair<string,int>> population;
    generatePopulation(population,populationSize,chromSize,objVec,maxWeight);
    while(numIterations--){

        sort(population.begin(),population.end(),[](auto &left, auto &right){return left.second > right.second;});
        pair<string,int> parent1 = population[rand()%populationSize];
        pair<string,int> parent2 = population[rand()%populationSize];
        int crossoverIdx = rand()%chromSize;
        string child1 = parent1.first.substr(0,crossoverIdx) + parent2.first.substr(crossoverIdx);
        string child2 = parent2.first.substr(0,crossoverIdx) + parent1.first.substr(crossoverIdx);
        population.pop_back(),population.pop_back();

        mutate(child1,child2);

        int fit1 = calcFit(child1,objVec,maxWeight);
        int fit2 = calcFit(child2,objVec,maxWeight);
        population.pb({child1,fit1}),population.pb({child2,fit2});
    }

    for(auto it : population){
        cout<<it.first<<" "<<it.second<<endl;
    }

}


int main(){

    int populationSize,numIterations,maxWeight;
    string line,subLine;
    ifstream inp("proposedProblem.csv",ifstream::in);
    if(!adminMode){
        cout<<"Please Enter the Population Size\n";
        cin>>populationSize;

        cout<<"Please Enter the Number of Iterations\n";
        cin>>numIterations;

        cout<<"Please Enter the Maximum Weight\n";
        cin>>maxWeight;

    }else{
        getline(inp,line);
        getline(inp,line,',');
        populationSize = stoi(line);
        getline(inp,line,',');
        numIterations = stoi(line);
        getline(inp,line);
        maxWeight = stoi(line);
        getline(inp,line);
    }

    vector<object*> objVec;
    while(getline(inp,line)){
        object* obj = new object;
        stringstream ss(line);
        getline(ss,subLine,',');
        obj->name = subLine;
        getline(ss,subLine,',');
        obj->weight = stoi(subLine);
        getline(ss,subLine,',');
        obj->price = stoi(subLine);
        objVec.pb(obj);
    }
    knapSack(objVec,populationSize,numIterations,maxWeight);
}
