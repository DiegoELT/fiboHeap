#include <iostream>
#include <list>
#include <utility>
#include <vector>
#include <cmath>
#include <fstream>
#include <chrono>
#include "CImg.h"
#include <map>

class Imagen {
private:
    std::string ruta;
    std::vector <float> vc;

public:
    const std::string getRuta() const {
        return ruta;
    }

    void setRuta(const std::string &ruta) {
        Imagen::ruta = ruta;
    }

    std::vector<float> getVc() {
        return vc;
    }

    void setVc(const std::vector<float> &vc) {
        Imagen::vc = vc;
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

    void setOrigen(const std::string &origen) {
        Arista::origen = origen;
    }

    const std::string &getDestino() const {
        return destino;
    }

    void setDestino(const std::string &destino) {
        Arista::destino = destino;
    }

    float getPeso() const {
        return peso;
    }

    void setPeso(float peso) {
        Arista::peso = peso;
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

    NodoFibonacci<T>* getMinimo() {
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

    std::vector <Arista> Kruskal() {
        //mapa de cada imagen, con su padre y el rango del padre.
        std::map<std::string, std::pair <std::string, int>> sets;
        std::vector<Arista> MST;
	    auto n = size;
        while (MST.size() != ((1+std::sqrt(1+8*n))/2)-1 && size > 1 /*&& MST.size() <= ((1+std::sqrt(1+8*n))/2)-4*/) {
            Arista arista = (getMinimo())->obtenerArista();
            extractMin();

            auto origen = arista.getOrigen();
            auto destino = arista.getDestino();

            auto it = sets.find(origen);
            auto ot = sets.find(destino);

            if( it != sets.end() ){}else{sets[origen] = make_pair(origen, 0);}
            if( ot != sets.end()){}else{sets[destino] = make_pair(destino, 0);}

            auto padreOrigen = sets[origen].first;
            auto padreDestino = sets[destino].first;
            while(sets[padreOrigen].first != padreOrigen) {
                padreOrigen = sets[padreOrigen].first;
            }

            while(sets[padreDestino].first != padreDestino){
                padreDestino = sets[padreDestino].first;
            }

            if(padreOrigen != padreDestino){
                MST.push_back(arista);
                if (sets[padreOrigen].second > sets[padreDestino].second){
                    sets[padreDestino].first = padreOrigen;
                } else{
                    sets[padreOrigen].first = padreDestino;
                }

                if(sets[padreOrigen].second == sets[padreDestino].second){
                    sets[padreDestino].second++;
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

float distEuclideana(std::vector <Imagen> &imagenes, int i, int j){
    float suma = 0;
    for (int k = 0; k < imagenes[i].getVc().size(); k++) {
        suma += (float)pow(imagenes[i].getVc()[k] - imagenes[j].getVc()[k], 2);
    }
    auto resultado = (float)std::sqrt(suma);
	return resultado;
}

float distChebyshov(std::vector <Imagen> &imagenes, int i, int j) {
	float resultado = 0;
	float suma;
	for (int k = 0; k < imagenes[i].getVc().size(); k++) {
        suma = imagenes[i].getVc()[k] - imagenes[j].getVc()[k];
        if (suma > resultado) resultado = suma;
    }
	return resultado;
}

float distMinkowski(std::vector <Imagen> &imagenes, int i, int j) {
    float suma = 0;
    for (int k = 0; k < imagenes[i].getVc().size(); k++) {
        suma += (float)pow(imagenes[i].getVc()[k] - imagenes[j].getVc()[k], log2(imagenes[i].getVc().size()));
    }
    float resultado = (float)pow(suma, 1/log2((double)imagenes[i].getVc().size()));
    return resultado;
}

void exportar(std::vector <Imagen> &imagenes, std::vector <Arista> &aristas) {
    std::fstream graphviz;
    std::string grafo;
    graphviz.open("../grafo.gv", std::ios::out);
    if (!graphviz.is_open()) {
        std::cout << "Error al exportar archivo" << std::endl;
        return;
    }
    grafo = "digraph {\n";
    for (const auto& it : imagenes) {
        grafo += "\t\"" + it.getRuta() + "\" [\n";
        grafo += "\t\timage = \"" + it.getRuta() + "\",\n";
        grafo += "\t\tlabel = \"\"\n";
        grafo += "\t];\n";
    }
    for (const auto& it : aristas) {
        grafo += "\t\"" + it.getOrigen() + "\" -> \"" + it.getDestino() + "\"\n";
    }
    grafo += "}";
    graphviz << grafo;
    graphviz.close();
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

    auto start = std::chrono::high_resolution_clock::now();

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

	        //float resultado = distEuclideana(imagenes, i, j);
	        //float resultado = distChebyshov(imagenes, i, j);
	        float resultado = distMinkowski(imagenes, i, j);
	        arista.setPeso(resultado);
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

    std::vector <Arista> aristas2 = heapFibonacci.Kruskal();
    exportar(imagenes, aristas2);

    auto stop = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);

    std::cout << "Duracion: " << duration.count() << " seconds";

    system("dot -Tpng -o ../grafo.png ../grafo.gv");
    system("display ../grafo.png");

    return 0;
}
