#include "network.h"
#include "random.h"
#include <iostream>
#include <algorithm>
#include <random>
using namespace std;



void Network::resize(const size_t& wish_size){
  values.resize(wish_size);
  RNG.normal(values);
}
bool Network::add_link(const size_t& n, const size_t& p){
  size_t taille(size());

  if(n>=taille or p>=taille or p==n or n<0 or p<0){
    return false;
  }

  vector<size_t> ngbrs(neighbors(n));
  for (size_t i(0); i<ngbrs.size(); i++){
    if(ngbrs[i]==p){
      return false;
    }
  }
  links.insert(std::make_pair(p, n));
  links.insert(std::make_pair(n, p));
  return true;
}

size_t Network::random_connect(const double& mean_deg){
  links.clear();
  size_t r(0); //counts the nodes, will be returned
  size_t nbr_links;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, size());

  for(size_t i(0); i<size(); i++){
      nbr_links =RNG.poisson(mean_deg);
      for(size_t y(0); y<nbr_links; y++){
        size_t newPosition(dis(gen));
        links.insert(std::make_pair(i, newPosition));
        links.insert(std::make_pair(newPosition, i));
        r++;
      }
  }
  return r;
}

size_t Network::set_values(const std::vector<double>& new_vector){
  size_t new_vector_size(new_vector.size());
  size_t val_size(values.size());
  if(new_vector_size==val_size){
    values.clear();
    values=new_vector;
    return val_size;
  }
  if(new_vector_size>val_size){
    for(size_t i(0); i<val_size;i++){
      values[i]=new_vector[i];
    }
    return val_size;
  }
  if(new_vector_size<val_size){
    for(size_t i(0); i<new_vector_size; i++){
      values[i]=new_vector[i];
    }
    return new_vector_size;
  }
  return 3000; //to recognize the error
}
size_t Network::size() const{
  return values.size();
}
size_t Network::degree(const size_t &_n) const{
  return links.count(_n);
}
double Network::value(const size_t &_n) const{
  return values[_n];
}
std::vector<double> Network::sorted_values() const{
  std::vector<double> result(values);
  std::sort(result.begin(), result.end(), greater<double>());
  return result;
}

std::vector<size_t> Network::neighbors(const size_t& n) const{
  std::vector<size_t> return_vector;
   for (auto I = links.begin(); I != links.end(); ++I){
      if(I->first == n){
        return_vector.push_back(I->second);
      }
   }
  return return_vector;
}
