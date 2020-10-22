#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <cmath>
#include <fstream>
#include "CImg.h"
#include <unordered_map>

class Imagen {
private:
    std::string ruta;
    std::vector <float> vc;

public:
    const std::string &getRuta() const {
        return ruta;
    }

    void setRuta(const std::string &string) {
        Imagen::ruta = string;
    }

    const std::vector<float> &getVc() const {
        return vc;
    }

    void setVc(const std::vector<float> &vector) {
        Imagen::vc = vector;
    }
};

class Arista {
private:
    std::string origen;
    std::string destino;
    float peso{};

public:
    const std::string &getOrigen() const {
        return origen;
    }

    void setOrigen(const std::string &string) {
        Arista::origen = string;
    }

    const std::string &getDestino() const {
        return destino;
    }

    string getOrigen(){return origen;}

    string getDestino(){return destino;}

    void setDestino(const std::string &string) {
        Arista::destino = string;
    }

    float getPeso() const {
        return peso;
    }

    void setPeso(float d) {
        Arista::peso = d;
    }
};

template <typename T>
class NodoFibonacci{
private:
    T m_dato{};
    Arista arista{};
    std::list<NodoFibonacci<T> *> m_hijos;
public:
    explicit NodoFibonacci(T m_dato, Arista arista1) : m_dato(m_dato), arista(std::move(arista1)) {};
    T obtenerDato(){
        return this -> m_dato;
    }

    Arista obtenerArista(){ return arista;}

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
            if (gradeCounter[(*it)->obtenerRango()] != m_heap.end() && gradeCounter[(*it)->obtenerRango()] != it) {
                int n = (*it)->obtenerRango();
                // std::cout << "El grado del elemento: " << (*it)->obtenerDato() << " y " << (*gradeCounter[(*it)->obtenerRango()])->obtenerDato() << " son iguales \n";
                it = unir(it, gradeCounter[(*it)->obtenerRango()]);
                // std::cout << "El iterador se encuentra en: " << (*it)->obtenerDato() << std::endl;
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
        size--;
        // printTree();
        compactar();
        m_pMin = m_heap.end();
        for (auto it = m_heap.begin(); it != m_heap.end(); it++){
            if(m_pMin == m_heap.end() || (*m_pMin) -> obtenerDato() > (*it) -> obtenerDato())
                m_pMin = it;
        }
    }

    typename std::list<NodoFibonacci<T>*>::iterator getMin(){
        return m_pMin;
    }

    void insert(T dato, const Arista& arista){
        auto* nuevoNodo = new NodoFibonacci<T>(dato, arista);
        this -> m_heap.push_back(nuevoNodo);
        if(m_pMin == m_heap.end() || (*m_pMin) -> obtenerDato() > nuevoNodo -> obtenerDato())
        {
            m_pMin = m_heap.end(); // Como es el ultimo insert, es el ultimo valor de la lista
            m_pMin--;
        }
        size++;
    }

    NodoFibonacci<T>* getMin(){
	return *m_pMin;
    }

    void printTree(){
        for(auto elem : m_heap)
            std::cout << elem -> obtenerDato() << "," << elem -> obtenerRango() << " -> ";
        std::cout << "\n";
    }

    int getSize(){
        return size;
    }

    vector<Arista> Kruskal(){
	//mapa de cada imagen, con su padre y el rango del padre.
    	unordered_map<string, pair<string, int> sets;
	vector<Arista> MST;
	
	while(size > 0){
		Arista arista = (getMin())->obtenerArista();
		extractMin();

		auto origen = arista.getOrigen();
                auto destino = arista.getDestino();

		if(sets[origen]){}else{sets[destino] = make_pair(origen, 0)}
		if(sets[destino]){}else{sets[destino] = make_pair(destino, 0)}

		if(sets[origen].first != sets[destino].first){
			MST.pushback(arista);
			if(sets[origen].second > sets[destino].second ){
				sets[destino].first = origen;
			}else{
				sets[origen].first = destino;
			}

			if(sets[origen].second == sets[destino].second){
				sets[destino].second++;
			}
		}
	}
	
	return MST;
	}
};

std::vector <float> vectorizar(cimg_library::CImg <float> &img) {
    std::vector <float> R;
    cimg_forXY(img, x, y) {
            R.emplace_back((img(x,y,0) + img(x,y,1) + img(x,y,2)) / 3);
        }
    return R;
}

int main() {
    // Test de estres
    /*HeapFibonacci<int> testFib;
    for(int i = 10000; i >= 1; i--){
        testFib.insert(i);
    }
    for(int i = 10000; i >= 2; i--){
        std::cout << (*testFib.getMin())->obtenerDato() << std::endl;
        testFib.extractMin();
    }
    std::cout << (*testFib.getMin())->obtenerDato() << std::endl;
    return 0;*/

    system("ls ../prueba/ \"*.jpg\" > ../database.txt");

    std::vector <Arista> aristas;

    std::vector <Imagen> imagenes;

    std::ifstream rutas("../database.txt");
    std::string ruta;
    getline(rutas, ruta);
    while (getline(rutas, ruta)) {
        //cout << "../imagenes/" << ruta << endl;
        std::string temp = "../prueba/" + ruta;
        cimg_library::CImg <float> A(temp.c_str());
        A.resize(128,128);
        //cout << A._width << A._height << endl;
        cimg_library::CImg <float> B = A.haar(false, 2);
        cimg_library::CImg <float> C = B.crop(0,0,27,27);

        std::vector <float> vc = vectorizar(C);

        Imagen imagen;
        imagen.setRuta(temp);
        imagen.setVc(vc);

        imagenes.emplace_back(imagen);

        //A.display();
        //B.display();
        //C.display();
    }

    for (int i = 0; i < imagenes.size()-1; i++) {
        for (int j = i+1; j < imagenes.size(); j++) {
            Arista arista;
            arista.setOrigen(imagenes[i].getRuta());
            arista.setDestino(imagenes[j].getRuta());
            float suma = 0;
            for (int k = 0; k < imagenes[i].getVc().size(); k++) {
                suma += (float)pow(imagenes[i].getVc()[k] - imagenes[j].getVc()[k], 2);
            }
            arista.setPeso((float)std::sqrt(suma));
            aristas.emplace_back(arista);
        }
    }

    HeapFibonacci <float> heapFibonacci;
    for (const auto& it : aristas) {
        heapFibonacci.insert(it.getPeso(), it);
    }
    heapFibonacci.printTree();
    std::cout << (*heapFibonacci.getMin())->obtenerDato() << std::endl;
    heapFibonacci.extractMin();
    std::cout << (*heapFibonacci.getMin())->obtenerDato() << std::endl;
    heapFibonacci.printTree();

    return 0;
}
