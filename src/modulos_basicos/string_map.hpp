
#include "string_map.h"

template <typename T>
string_map<T>::string_map(){
    this->raiz = new Nodo(nullptr);
    this->_size = 0;
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    for(string clave : this->_definidas) this->erase(clave);
    for(string clave : d._definidas) {
        auto valueToInsert = d.at(clave);
        auto pair = make_pair(clave, valueToInsert);
        this->insert(pair);
    }
}

template <typename T>
string_map<T>::~string_map(){
    this->raiz->borrarSiguientes();

    delete this->raiz;
}

template <typename T>
T& string_map<T>::operator[](const string& clave) {
    return *(this->nodoAt(clave)->definicion);
}

template <typename T>
int string_map<T>::count(const string& clave) const{
    Nodo *actual = this->raiz;
    for(unsigned int i = 0; i < clave.length(); i++) {
        char proxChar = clave.at(i);
        Nodo* siguiente = actual->siguientes[proxChar];
        if(siguiente == nullptr) {
            return 0;
        }
        actual = siguiente;
    }
    
    return actual->definicion ? 1 : 0;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    return *(this->nodoAt(clave)->definicion);
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    return (*this)[clave];
}


template <typename T>
void string_map<T>::erase(const string& clave) {
    this->_size--;
    this->_definidas.erase(clave);
    this->optionalErase(clave, true);
}

template<typename T>
void string_map<T>::optionalErase(const string &key, bool esObjetivo) {
    if(key.empty() || (this->nodoAt(key)->definicion && !esObjetivo)) return;

    Nodo *pNodo = this->nodoAt(key);
    string shorterKey = key.substr(0, key.length() - 1);
    if(!pNodo->tieneSiguientes()) {
        delete pNodo;
        this->nodoAt(shorterKey)->siguientes[key[key.length() - 1]] = nullptr;
    } else {
        delete pNodo->definicion;
        pNodo->definicion = nullptr;
    }

    this->optionalErase(shorterKey, false);
}

template <typename T>
int string_map<T>::size() const{
    return this->_size;
}

template <typename T>
bool string_map<T>::empty() const{
    return this->size() == 0;
}

template<typename T>
void string_map<T>::insert(const pair<string, T> & pair) {
    string key = pair.first;
    T *pT = new T(pair.second);

    Nodo *actual = this->raiz;
    for (int i = 0; i < key.length(); i++) {
        Nodo *siguiente = actual->siguientes[key[i]];
        if (!siguiente) {
            actual->siguientes[key[i]] = new Nodo(nullptr);
        }
        actual = actual->siguientes[key[i]];
    }

    if(actual->definicion) delete actual->definicion;
    else this->_size++;

    actual->definicion = pT;
    this->_definidas.insert(key);
}

template<typename T>
class string_map<T>::Nodo *string_map<T>::nodoAt(const string &key) const {
    Nodo* actual = this->raiz;
    for(int i = 0; i < key.length(); i++) {
        auto nextChar = key[i];
        actual = actual->siguientes[nextChar];
    }

    return actual;
}

template<typename T>
bool string_map<T>::operator==(const string_map &r) const {
    if(this->_definidas != r._definidas) return false;

    for(auto def : this->_definidas) {
        this->at(def) == r.at(def);
    }
}

template<typename T>
vector<string> string_map<T>::claves() {
    vector<string> v(this->_definidas.size());
    for(auto def : this->_definidas) v.push_back(def);
    return v;
}

template<typename T>
string_map<T>::Nodo::Nodo(T* definicion) : definicion(definicion) {
    this->siguientes = vector<Nodo*>('z');
}

template<typename T>
string_map<T>::Nodo::~Nodo() {
    delete this->definicion;
    for(auto sig : this->siguientes) delete sig;
}

template<typename T>
bool string_map<T>::Nodo::tieneSiguientes() const {
    bool ret = false;
    for(auto sig : this->siguientes) {
        if(sig) ret = true;
    }
    return ret;
}

template<typename T>
void string_map<T>::Nodo::borrarSiguientes() {
    for(int i = 0; i < this->siguientes.size(); i++) {
        auto sig = this->siguientes[i];
        if(sig) {
            sig->borrarSiguientes();
            delete sig;
            this->siguientes[i] = nullptr;
        }
    }
}
