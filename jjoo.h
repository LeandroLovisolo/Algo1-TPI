#ifndef JJOO_H
#define JJOO_H

#include "competencia.h"

typedef pair<int, pair<int, int> > infoM;

class JJOO{
    public:

        JJOO();
        // |cronograma|==cantDias. Si un d�a no hay competencias,q est� la lista vac�a.
        JJOO(const int anio, const Lista<Atleta>& atletas, const Lista<Lista<Competencia> >& competenciasPorDia);
        int anio() const;
        Lista<Atleta> atletas() const;
        int cantDias() const;
        int jornadaActual() const;
        Lista<Competencia> cronograma(const int dia) const;

        Lista<Competencia> competencias() const;
        Lista<Competencia> competenciasFinalizadasConOroEnPodio() const;

        Lista<Atleta> dePaseo() const;
        Lista<pair<Pais,Lista<int> > > medallero() const;
        int boicotPorDisciplina(const Categoria cat, const Pais p);
        Lista<Atleta> losMasFracasados(const Pais p) const;
        void liuSong(const Atleta& a, const Pais p);
        Atleta stevenBradbury() const;
        bool uyOrdenadoAsiHayUnPatron() const;
        Lista<Pais> sequiaOlimpica() const;
        void transcurrirDia();

		bool operator==(const JJOO& j) const;

        void mostrar(std::ostream& os) const;
		void guardar(std::ostream& os) const;
		void cargar (std::istream& is);

    private:
        int _anio;
        Lista<Atleta> _atletas;
        int _jornadaActual;
        Lista<Lista<Competencia> > _competenciasPorDia;         // En la i-�sima posici�n de la lista, las competencias del d�a i+1.
        Lista<Atleta> participantesJJOO(const Lista<Competencia> competencias) const;
        bool mismosAtletas(const JJOO& c) const;
        bool mismoCronograma(const JJOO& j) const;
        Lista<int> rank(Competencia c);
        int maxPos(Lista<pair<int,int> > a, int desde, int hasta);
        Lista<pair<int, bool> > doping(const Competencia c);
        Lista<Lista<Competencia> > m(Lista<Lista<Competencia> > h, Lista<Competencia> comp, int w);
        Lista<pair<int,int> > capacidades(const Deporte d , Lista<Atleta> atle);
        Lista<pair<int,int> > swap(Lista<pair<int,int> > lista, int a, int b);
        Lista<int> crearRanking(const Competencia& c);
        bool ganoMedallaEseDia(Pais p, int x) const;
        int maxDiasSinGanar(Lista<int> lista) const;
        enum {ENCABEZADO_ARCHIVO = 'J'};
};
std::ostream & operator<<(std::ostream & os,const JJOO & j);

#endif // JJOO_H

