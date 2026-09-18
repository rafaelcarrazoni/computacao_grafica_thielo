#include <bits/stdc++.h>

using namespace std;

class Imagem 
{
    public:

        string tipo_arquivo;
        int altura;
        int largura;
        int max_color;
    
        vector<vector<int>> vermelho;
        vector<vector<int>> verde;
        vector<vector<int>> azul;

        Imagem(int altura, int largura)
        {
            this->altura = altura * 16;
            this->largura = largura * 16;
            this->tipo_arquivo = "P3";
            this->max_color = 255;

            vermelho.resize(this->altura);
            verde.resize(this->altura);
            azul.resize(this->altura);

            for(int i = 0; i < this->altura; i++)
            {
                vermelho[i].resize(this->largura);
                verde[i].resize(this->largura);
                azul[i].resize(this->largura);
            }
            
        }

        Imagem(string caminho)
        {
            ifstream imagem(caminho);


            imagem >> tipo_arquivo;
            imagem >> altura;
            imagem >> largura;
            imagem >> max_color;

            vermelho.resize(altura);
            verde.resize(altura);
            azul.resize(altura);

            for(int i = 0; i < altura; i++)
            {
                vermelho[i].resize(largura);
                verde[i].resize(largura);
                azul[i].resize(largura);
            }

            for(int i = 0; i < altura; i++)
            {
                for(int j = 0; j < largura; j++)
                {
                    imagem >> vermelho[i][j];
                    imagem >> verde[i][j];
                    imagem >> azul[i][j];
                }
            }


        }

        void printar(string destino)
        {
            ofstream arquivo(destino);

            arquivo << tipo_arquivo << endl;
            arquivo << altura << " " << largura << endl;
            arquivo << max_color << endl;

            for(int i = 0; i < altura; i++)
            {
                for(int j = 0; j < largura; j++)
                {
                    arquivo << vermelho[i][j] << " ";
                    arquivo << verde[i][j] << " ";
                    arquivo << azul[i][j] << " ";
                }
                arquivo << endl;
            }
        }

        void set_pixel(int alturap, int largurap, const vector<int>& color)
        {


                if(alturap >= altura || largurap >= largura || alturap < 0|| largurap < 0)
                {
                    return;
                }

                vermelho[alturap][largurap] = color[0];
                verde[alturap][largurap] = color[1];
                azul[alturap][largurap]  = color[2];

        }

        int get_red(int altura, int largura)
        {
            return this->vermelho[altura][largura];
        }
        int get_green(int x, int y)
        {
            return this->verde[x][y];
        }
        int get_blue(int a, int b)
        {
            return this->azul[a][b];
        }
        vector<int> get_pixel(int x, int y)
        {
            vector<int> pixel;
            pixel.push_back(vermelho[x][y]);
            pixel.push_back(verde[x][y]);
            pixel.push_back(azul[x][y]);

            return pixel;
        }

        void carimbar(Imagem* carimbo, int x, int y, bool ignorar_transparente = false, const vector<int>& transparente = {})
        {
            for(int i = 0; i < carimbo->altura && (x + i) < this->altura; i++)
            {
                for(int j = 0; j < carimbo->largura && (y + j) < this->largura; j++)
                {

                    if(ignorar_transparente)
                    {
                        if(carimbo->vermelho[i][j] == transparente[0] && carimbo->verde[i][j] == transparente[1] && carimbo->azul[i][j] == transparente[2])
                        {
                            continue;
                        }
                    }

                    this->vermelho[x + i][y + j] = carimbo->vermelho[i][j];
                    this->verde[x + i][y + j]    = carimbo->verde[i][j];
                    this->azul[x + i][y + j]     = carimbo->azul[i][j];
                }
            }
        }
        void carimbar_mapa(string mapa_string, Imagem arbusto, Imagem chao)
        {
            ifstream mapa(mapa_string);
            char caracter;
            string linha;
            for(int i = 0 ; i < this->altura/16; i++){
                mapa >> linha;
                for(int j = 0; j < this->largura/16; j++){
                    caracter = linha[j];
                    if(caracter == '#'){
                        this->carimbar(&arbusto,i * 16,j *16);
                    }
                    else 
                    {
                        this->carimbar(&chao, i * 16, j * 16);
                    }
                }
            }
        }
        // Cálculos para filtros
        float grey(int x,int y)
        {
            return (30.*get_red(x, y)+59.*get_green(x, y)+11.*get_blue(x,y))/100.;
        }

        int flo(float a)
        {
            if (a<128)return 0;
            else return 255;
        }
};

class Filtro 
{
    public:
        static Imagem dither(Imagem original)
        {
            for (int x=1; x<(original.largura-1); x++)
            {
                for (int y=1; y<(original.altura-1); y++)
                {
                    cout << "X: " << x << " Y: " << y << endl;
                    int P  = original.grey(x,y);   // tom de cinza do pixel atual
                    int newpixel  = original.flo(P);  //  binariza para branco ou preto
                    float error= P-newpixel;  // obtem o erro, que eh a diferenca entre o tom de cinza e o binarizado
                    float alpha=7./16.; // pesos para a difusao do erro
                    float beta=3./16.; //
                    float gamma=5./16; //
                    float delta=1.0/16.; //

                    int a=(int)(original.grey(x,y+1)+error*alpha);
                    int b=(int)(original.grey(x+1,y-1)+error*beta);
                    int c=(int)(original.grey(x+1,y)+error*gamma);
                    int d=(int)(original.grey(x+1,y+1)+error*delta);

                    original.set_pixel(x,y,{newpixel,newpixel,newpixel});
                    original.set_pixel(x, y+1,{a,a,a});//=  7 / 16
                    original.set_pixel(x + 1,y - 1,{b,b,b});
                    original.set_pixel(x +1,y ,{c,c,c});
                    original.set_pixel(x+1,y+1,{d,d,d});

                }
                cout << endl;
            }
            return original;
        }

};

void atividade_carimbar_labirinto()
{
    Imagem arbusto = Imagem("Atividade_1_09/arbusto.ppm");
    Imagem chao = Imagem("Atividade_1_09/chao.ppm");

    Imagem alvo = Imagem(59, 118);

    alvo.carimbar_mapa("Atividade_1_09/maze.txt", arbusto, chao);

    alvo.printar("maze.ppm");   
}

void atividade_dither()
{
    Imagem carinhaReborn = Imagem("Atividade_1_09/carinhaReborn.ppm");
    Imagem reborn = Filtro::dither(carinhaReborn);
    reborn.printar("carinhaDithered.ppm");
}

int main()
{
    atividade_dither();
    return 0;
}