#include <iostream>
#include <random>
#include <time.h>
#include <windows.h>

using namespace std;

//////////////////random functions/////////////////////

double randf(){ // returns a value between 0 and 1
    double res = rand() % 1001;
    res /= 1000;
    return res;
}


// map a value
double map(double value, double min1, double max1, double min2, double max2){
    return min2+(max2-min2)*((value-min1)/(max1-min1));
}


double random(double max){ // returns a random value between 0 and max
    return randf() * max;
}


double random(double min,double max){ // returns a random value between min and max
    return map(randf(),0,1,min,max);
}



char rand_char(){ // returns a random charactor
    return (int) random(32,126);
}



////////////// main variables /////////////////////

const int pop_count = 100; // population number for each generation

string target = "to be or not to be"; // target phrase
int generations = 0; // generations count

double mutation_rate = 1; // percentage

double total_fitness;  // total fitness of the generation


// DNA class reperesents a one population
class DNA{
    string genes;   // the phrase of the DNA
    double fitness; // the fitness of the DNA
public:
    DNA(){// default constructor

    }
    DNA(string gen){ // parametrized constructor will receive a phrase to assign it to the phrase of the DNA
        genes = gen;
    }

    void random_genes(){ // make a random phrase to the DNA
        for(int i = 0; i < target.length();i++){
            genes += rand_char();
        }
    }

    string get_genes(){ // returns the genes of the DNA
        return genes;
    }

    void calculate_fitness(){ // calculates the fitness of the DNA
        double score = 0; // score will begin at 0
        for(int i = 0; i < genes.length();i++){
            // if the genes character at the index (i) is the same as the target character at the same index
            // the score will increase
            if(genes[i] == target[i])
                score++;
        }
        fitness = score/genes.length();// the fitness is calculated by dividing the score with the total
                                       //length of the phrase and will be a value between 0 and 1
    }

    void mutate(){ // mutates the genes of the DNA
        for(int i = 0; i < target.length();i++){
            double mutation_chance = random(100);
            if(mutation_chance <= mutation_rate){
                genes[i] = rand_char();
            }
        }
    }


    double get_fitness(){ // returns the fitness value
        return fitness;
    }
};



DNA crossover(DNA parentA, DNA parentB){ // making a new child population element from two parents
    string child_genes = "";
    for(int i = 0; i < parentA.get_genes().length();i++){
        int chance = round(random(1)); // random 0 or 1
        if(chance == 0){ // if 0 take the character from parentA
            child_genes += parentA.get_genes()[i];
        }else{           // if 1 take the character from parentB
            child_genes += parentB.get_genes()[i];
        }
    }
    DNA child = DNA(child_genes);
    return child;
}


void mutate(DNA pop[]){ // mutate the population
    for(int i = 0; i < pop_count;i++){
        pop[i].mutate();
    }
}



// evaluating
DNA calculate_fitness(DNA pop[]){
    // calculating fitness for each element in the population
    // and finding the highest fitness element
    double t_fitness = 0;
    DNA best = pop[0];
    for(int i = 0;i < pop_count;i++){
        pop[i].calculate_fitness();
        t_fitness += pop[i].get_fitness();
        if(pop[i].get_fitness() > best.get_fitness()){
            best = pop[i];
        }
    }
    total_fitness = t_fitness/pop_count;
    total_fitness *= 100;// convert it to a percentage
    return best;
}



// making a mating pool
void matingPool(DNA pop[]){
    // making the mating pool according to the fitness of every population
    // element we will add the element few times to make the higher the fitness
    // the higher chance the of the element to be picked to be a parent
    // without leaving no chance to low fitness population
    vector<DNA> mating_pool;
    for(int i = 0; i < pop_count;i++){
        int probablity = round(random(pop[i].get_fitness() * 10)) + 1;
        for(int j = 0;j < probablity;j++){
            mating_pool.push_back(pop[i]);
        }
    }

    // making the new generation
    DNA new_population[pop_count];


    // making a cross over from the mating pool with random parents
    for(int i = 0; i < pop_count;i++){
        int indexA = floor(random(mating_pool.size()));
        int indexB = floor(random(mating_pool.size()));

        if(indexA >= mating_pool.size()){
            indexA = mating_pool.size() - 1;
        }

        if(indexB >= mating_pool.size()){
            indexB = mating_pool.size() - 1;
        }


        DNA parentA = mating_pool.at(indexA);
        DNA parentB = mating_pool.at(indexB);
        DNA child = crossover(parentA,parentB);
        new_population[i] = child;
    }
    // assigning the new population to the old
    for(int i = 0; i < pop_count;i++){
        pop[i] = new_population[i];
    }
    //increasing the generations variable
    generations++;
}




DNA population[pop_count]; // the main population








void setup(){ // runs once at the start of the program

    //intialize population with random genes
    for(int i = 0; i < pop_count;i++){
        population[i].random_genes();
    }
}


bool loop(){ // runs 30 times in a second if returns false the program stops

    system("CLS"); // clearing the screen

    // calculating the fitness and printing the best
    DNA best = calculate_fitness(population);

    // displaying information
    cout<<"TARGET PHRASE"<<endl;
    cout<<target<<endl<<endl;

    cout<<"best phrase:"<<endl<<endl;
    cout<<"         "<<best.get_genes()<<endl<<endl;

    cout<<"best fitness: "<<best.get_fitness()*100<<"%"<<endl<<endl;

    cout<<"total fitness: "<< total_fitness<<"%"<<endl;
    cout<<"total generations: "<<generations<<endl;



    // if the best element is matching the target phrase the program stops
    if(best.get_genes() == target){
        return false;
    }


    // making the mating pool and a new generation
    matingPool(population);

    // mutating the new population
    mutate(population);

    return true;
}


int main()
{
    srand(time(NULL)); // initialize a random seed
    setup();




    //loop runs 30 time in a second
    while(loop()){
       Sleep(1000/30);// time delay
    }

}
