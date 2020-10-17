#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

template <typename T>
class NodoFibonacci{
  private:
    T m_dato{};
    std::list<NodoFibonacci<T> *> m_hijos{};
  public:
    NodoFibonacci(T m_dato) : m_dato(m_dato) {};
    T obtenerDato(){
      return this -> m_dato;
    }
    T obtenerRango(){
      return this -> m_hijos.size();
    }
    std::list<NodoFibonacci<T> *> obtenerHijos(){
      return this-> m_hijos;
    }
    void giveChild(NodoFibonacci<T> * child){
      m_hijos.push_back(child);
    }
};

template <typename T>
class HeapFibonacci{
  private:
    int size = 0;
    std::list<NodoFibonacci<T> *> m_heap;
    typename std::list<NodoFibonacci<T>*>::iterator m_pMin = m_heap.end();
    
  public:
    typename std::list<NodoFibonacci<T>*>::iterator unir(typename std::list<NodoFibonacci<T>*>::iterator nodeA, typename std::list<NodoFibonacci<T>*>::iterator nodeB){ 
      if ((*nodeA)->obtenerDato() <= (*nodeB)->obtenerDato()) {
        m_heap.erase(nodeB);
        (*nodeA)->giveChild(*nodeB);
        return nodeA;
      } else {
        m_heap.erase(nodeA);
        (*nodeB)->giveChild(*nodeA);
        return nodeB;
      }
    }
    
    void compactar(){
      std::vector<typename std::list<NodoFibonacci<T>*>::iterator> gradeCounter(log2(size)+1, m_heap.end());
      auto it = m_heap.end();
      it--;
      for (; it != m_heap.begin(); it--) {
        if (gradeCounter[(*it)->obtenerRango()] != m_heap.end()) {
          int n = (*it)->obtenerRango();
          it = unir(it, gradeCounter[(*it)->obtenerRango()]);
          it++;
          gradeCounter[n] = m_heap.end();
        } else{
          gradeCounter[(*it)->obtenerRango()] = it;
        }
      }
      while (gradeCounter[(*it)->obtenerRango()] != m_heap.end()) {
          it = unir(it, gradeCounter[(*it)->obtenerRango()]);
      }
    }
    
    void extractMin(){
      for (auto hijo : (*m_pMin)->obtenerHijos()) {
        m_heap.push_back(hijo);
      }
      m_heap.erase(m_pMin);
      compactar();
      m_pMin = m_heap.end();
      for (auto it = m_heap.begin(); it != m_heap.end(); it++){
        if(m_pMin == m_heap.end() || (*m_pMin) -> obtenerDato() > (*it) -> obtenerDato())
          m_pMin = it;
      }
      size--;
    }
    
    typename std::list<NodoFibonacci<T>*>::iterator getMin(){
      return m_pMin;
    }
    
    void insert(T dato){
      NodoFibonacci<T> * nuevoNodo = new NodoFibonacci(dato);
      this -> m_heap.push_back(nuevoNodo);
      if(m_pMin == m_heap.end() || (*m_pMin) -> obtenerDato() > nuevoNodo -> obtenerDato())
      {
        m_pMin = m_heap.end(); // Como es el ultimo insert, es el ultimo valor de la lista
        m_pMin--;
      }
      size++;
    }
    
    void printTree(){
      for(auto elem : m_heap)
        std::cout << elem -> obtenerDato() << "," << elem -> obtenerRango() << " -> ";
      std::cout << "\n";
    }
    
};

int main() {
	HeapFibonacci<int> testFib;
  testFib.insert(1);
  testFib.printTree();
  testFib.insert(2);
  testFib.printTree();
  testFib.insert(3);
  testFib.printTree();
  testFib.insert(-1);
  testFib.printTree();
  testFib.extractMin();
  testFib.printTree();
  std::cout << (*testFib.getMin())->obtenerDato() << std::endl;
	return 0;
}
