#include <string>
#include <iostream>
#include <vector>
#include <iomanip>

std::vector<std::pair<std::vector<int>, double> > equations[22];
//this program is for std::, yep


void calculate_step(double *current_count, double *result, int N) {
    for (int i = 0; i < N; i++)
        result[i] = 0;
    for (int i = 0; i < N; i++) 
        for (int j = 0; j < equations[i].size(); j++) {
            double value = 1.0;
            for (int idc : equations[i][j].first)
                value = value * current_count[idc];
            result[i] = result[i] + value*equations[i][j].second;
        }
}   


int main() {
  //init
  double reactor[22];
  double T;
  int N;
  int M;
  //reading part:
  //read time, number of reactants
  //read concentrations
  std::cin >> T >> N;
  for (int i = 0; i < N; i++)
      std::cin >> reactor[i];
  //read number of reactions
  std::cin >> M;
  //read each reaction:
  for (int i = 0; i < M; i++) {
      int count, id;  //tmp variables for reading
      std::vector<int> r_ids;  //for ids of reactants
      std::vector<int> counts;  //amount of them
      std::cin >> count >> id;  //first pair is read
      counts.push_back(count); //adding number and id
      r_ids.push_back(id);  
      std::string str;
      std::cin >> str;
      //std::cout << str << endl;
      if (str != ">") { 
        std::cin >> count >> id;
        counts.push_back(count);
        r_ids.push_back(id);
        std::cin >> str; //reading ">"
      }
      double k;
      std::cin >> k; //reading reaction rate
      if (counts.size() == 2) {
        equations[r_ids[0]].push_back({r_ids, -k});
        equations[r_ids[1]].push_back({r_ids, -k});
      } 
      else { 
        if (counts[0] == 2) { //we have both components (s.o.d.e.)
          r_ids.push_back(r_ids[0]);
          equations[r_ids[0]].push_back({r_ids, -2 * k});
        } 
        else 
          equations[r_ids[0]].push_back({r_ids, -k}); //only one
      }

      while (str != "#") {
        std::cin >> str; //read amount of reactant
        count = std::stold(str);
        std::cin >> str; //read reactant id
        equations[std::stoi(str)].push_back({r_ids, k * count});  //pushes <ids, float> to vector as the equation
        std::cin >> str; //skip + or end with #
      }
  }
  //using Runge-Kutta method
  double h = T/4000.0;
  double *c1 = new double[22];
  double *c2 = new double[22];
  double *c3 = new double[22];
  double *c4 = new double[22];
  double *temporary = new double[22];
  for (int step = 0; step < 4000; step++) {
    calculate_step(reactor, c1, N);
    for (int i = 0; i < N; i++) 
      temporary[i] = reactor[i] + (h/2)*c1[i];
    calculate_step(temporary, c2, N);
    for (int i = 0; i < N; i++) 
      temporary[i] = reactor[i] + (h/2)*c2[i];
    calculate_step(temporary, c3, N);
    for (int i = 0; i < N; i++) 
      temporary[i] = reactor[i] + h*c3[i];
    calculate_step(temporary, c4, N);
    for (int i = 0; i < N; i++) 
      reactor[i] += (h/6)*(c1[i] + 2*c2[i] + 2*c3[i] + c4[i]);
  }
  //output answer with e-6 accuracy
  for (int i = 0; i < N; i++)
    printf("%.8f ", reactor[i]);
  return 0;
}
