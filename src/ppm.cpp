#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

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

            for(int linha = 0; linha < this->altura; linha++)
            {
                vermelho[linha].resize(this->largura);
                verde[linha].resize(this->largura);
                azul[linha].resize(this->largura);
            }
            
        }

        Imagem(string caminho)
        {
            ifstream imagem(caminho);


            imagem >> tipo_arquivo;
            imagem >> largura;
            imagem >> altura;
            imagem >> max_color;

            vermelho.resize(altura);
            verde.resize(altura);
            azul.resize(altura);

            for(int linha = 0; linha < altura; linha++)
            {
                vermelho[linha].resize(largura);
                verde[linha].resize(largura);
                azul[linha].resize(largura);
            }

            for(int linha = 0; linha < altura; linha++)
            {
                for(int coluna = 0; coluna < largura; coluna++)
                {
                    imagem >> vermelho[linha][coluna];
                    imagem >> verde[linha][coluna];
                    imagem >> azul[linha][coluna];
                }
            }


        }

        void printar(string destino)
        {
            ofstream arquivo(destino);

            arquivo << tipo_arquivo << endl;
            arquivo << largura << " " << altura << endl;
            arquivo << max_color << endl;

            for(int linha = 0; linha < altura; linha++)
            {
                for(int coluna = 0; coluna < largura; coluna++)
                {
                    arquivo << vermelho[linha][coluna] << " ";
                    arquivo << verde[linha][coluna] << " ";
                    arquivo << azul[linha][coluna] << " ";
                }
                arquivo << endl;
            }
        }

        void set_pixel(int linha, int coluna, const vector<int>& color)
        {
                if(linha >= altura || coluna >= largura || linha < 0 || coluna < 0)
                {
                    return;
                }

                vermelho[linha][coluna] = color[0];
                verde[linha][coluna] = color[1];
                azul[linha][coluna]  = color[2];

        }

        int get_red(int linha, int coluna)
        {
            return this->vermelho[linha][coluna];
        }
        int get_green(int linha, int coluna)
        {
            return this->verde[linha][coluna];
        }
        int get_blue(int linha, int coluna)
        {
            return this->azul[linha][coluna];
        }
        vector<int> get_pixel(int linha, int coluna)
        {
            if(linha >= this->altura || linha < 0 || coluna >= this->largura || coluna < 0)
            {
                return {}; // retorna vazio quando o pixel não existe
            }

            vector<int> pixel;
            pixel.push_back(vermelho[linha][coluna]);
            pixel.push_back(verde[linha][coluna]);
            pixel.push_back(azul[linha][coluna]);

            return pixel;
        }

        void carimbar(Imagem* carimbo, int linha_inicial, int coluna_inicial, bool ignorar_transparente = false, const vector<int>& transparente = {})
        {
            for(int linha = 0; linha < carimbo->altura && (linha_inicial + linha) < this->altura; linha++)
            {
                for(int coluna = 0; coluna < carimbo->largura && (coluna_inicial + coluna) < this->largura; coluna++)
                {
                    if(ignorar_transparente)
                    {
                        if(carimbo->vermelho[linha][coluna] == transparente[0] && carimbo->verde[linha][coluna] == transparente[1] && carimbo->azul[linha][coluna] == transparente[2])
                        {
                            continue;
                        }
                    }

                    int linha_destino = linha_inicial + linha;
                    int coluna_destino = coluna_inicial + coluna;

                    this->vermelho[linha_destino][coluna_destino] = carimbo->vermelho[linha][coluna];
                    this->verde[linha_destino][coluna_destino]    = carimbo->verde[linha][coluna];
                    this->azul[linha_destino][coluna_destino]     = carimbo->azul[linha][coluna];
                }
            }
        }

        void carimbar(Imagem* carimbo, int linha_inicial, int coluna_inicial, int coluna_final, bool ignorar_transparente = true, const vector<int>& transparente = {0, 0, 0})
        {
            cout << "chegamos no carimbo";
            for(int linha = 0; linha < carimbo->altura && (linha_inicial + linha) < this->altura; linha++)
            {
                for(int coluna = 0; coluna < carimbo->largura - coluna_final && (coluna_inicial + coluna) < this->largura; coluna++)
                {
                    if(ignorar_transparente)
                    {
                        if(carimbo->vermelho[linha][coluna] == transparente[0] && carimbo->verde[linha][coluna] == transparente[1] && carimbo->azul[linha][coluna] == transparente[2])
                        {
                            continue;
                        }
                    }

                    int linha_destino = linha_inicial + linha;
                    int coluna_destino = coluna_inicial + coluna;

                    this->vermelho[linha_destino][coluna_destino] = carimbo->vermelho[linha][coluna];
                    this->verde[linha_destino][coluna_destino]    = carimbo->verde[linha][coluna];
                    this->azul[linha_destino][coluna_destino]     = carimbo->azul[linha][coluna];
                }
            }
        }

        

        void carimbar_mapa(string mapa_string, Imagem arbusto, Imagem chao)
        {
            ifstream mapa(mapa_string);
            char caracter;
            string linha_texto;
            for(int linha = 0 ; linha < this->altura/16; linha++){
                mapa >> linha_texto;
                for(int coluna = 0; coluna < this->largura/16; coluna++){
                    caracter = linha_texto[coluna];
                    if(caracter == '#'){
                        this->carimbar(&arbusto, linha * 16, coluna * 16);
                    }
                    else 
                    {
                        this->carimbar(&chao, linha * 16, coluna * 16);
                    }
                }
            }
        }
        // Cálculos para filtros
        float grey(int linha, int coluna)
        {
            return (30.*get_red(linha, coluna)+59.*get_green(linha, coluna)+11.*get_blue(linha, coluna))/100.;
        }

        int flo(float valor)
        {
            if (valor < 128) return 0;
            else return 255;
        }
};

class Filtro 
{
    public:
        static Imagem dither(Imagem original)
        {
            for (int linha = 1; linha < (original.altura - 1); linha++)
            {
                for (int coluna = 1; coluna < (original.largura - 1); coluna++)
                {
                    int P  = original.grey(linha, coluna);   // tom de cinza do pixel atual
                    int newpixel  = original.flo(P);  //  binariza para branco ou preto
                    float error = P - newpixel;  // obtem o erro, que eh a diferenca entre o tom de cinza e o binarizado
                    float alpha = 7./16.; // pesos para a difusao do erro
                    float beta = 3./16.; //
                    float gamma = 5./16; //
                    float delta = 1.0/16.; //

                    int a = (int)(original.grey(linha, coluna + 1) + error * alpha);
                    int b = (int)(original.grey(linha + 1, coluna - 1) + error * beta);
                    int c = (int)(original.grey(linha + 1, coluna) + error * gamma);
                    int d = (int)(original.grey(linha + 1, coluna + 1) + error * delta);

                    original.set_pixel(linha, coluna, {newpixel, newpixel, newpixel});
                    original.set_pixel(linha, coluna + 1, {a, a, a});//=  7 / 16
                    original.set_pixel(linha + 1, coluna - 1, {b, b, b});
                    original.set_pixel(linha + 1, coluna, {c, c, c});
                    original.set_pixel(linha + 1, coluna + 1, {d, d, d});

                }
            }
            return original;
        }

};

class Bresenham 
{
    public:
        static void desenhar(Imagem imagem, int x0, int y0, int x1, int y1)
        {
            int delta_y = abs(y1 - y0);
            int delta_x = abs(x1 - x0);

            int passo_x = x1 > x0 ? 1 : -1;
            int passo_y = y1 > y0 ? 1 : -1;

            if(delta_x >= delta_y){
                int y_atual = y0;
                for(int x_atual = x0; x_atual < x1; x_atual += passo_x)
                {
                    imagem.set_pixel(y_atual, x_atual, {255, 255, 255});
                    int pi = (2 * delta_y * x_atual - 2 * delta_x * y_atual) + (2 * delta_y - delta_x);

                    if(pi < 0)
                    {
                        pi = pi + 2 * delta_y;
                    }
                    else 
                    {
                        pi = pi + 2 * delta_y - 2 * delta_x;
                        y_atual += passo_y;
                    }

                }
                return;
            }

            int x_atual = x0;
            for(int y_atual = y0; y_atual < y1; y_atual += passo_y)
            {
                imagem.set_pixel(y_atual, x_atual, {255, 255, 255});
                int pi = (2 * delta_x * y_atual - 2 * delta_y * x_atual) + (2 * delta_x - delta_y);

                if(pi < 0)
                {
                    pi = pi + 2 * delta_x;
                }
                else 
                {
                    pi = pi + 2 * delta_x - 2 * delta_y;
                    x_atual += passo_x;
                }

            }


        }
};

class PaintBucket
{
    public:
    static void floodfill(Imagem imagem, int linha_inicial, int coluna_inicial, const vector<int>& cor_escolhida = {0, 0, 0})
    {
        vector<int> pixel_original = imagem.get_pixel(linha_inicial, coluna_inicial);

        if(pixel_original.empty())
        {
            return;
        }

        floodfill(imagem, linha_inicial, coluna_inicial, pixel_original, cor_escolhida);
    }
    private: 
    static void floodfill(Imagem imagem, int linha, int coluna, const vector<int>& pixel_original, const vector<int>& cor_escolhida)
    {
        set<pair<int, int>> visitados;
        stack<pair<int, int>> pilha;
        pilha.push({linha, coluna});

        while(!pilha.empty())
        {
            pair<int, int> atual = pilha.top();
            pilha.pop();

            int linha_atual = atual.first;
            int coluna_atual = atual.second;

            if(visitados.count(atual) > 0)
            {
                continue;
            }
            visitados.insert(atual);

            vector<int> pixel_atual = imagem.get_pixel(linha_atual, coluna_atual);
            if(pixel_atual.empty())
            {
                continue;
            }

            if(imagem.vermelho[linha_atual][coluna_atual] != pixel_original[0] || imagem.verde[linha_atual][coluna_atual] != pixel_original[1] || imagem.azul[linha_atual][coluna_atual] != pixel_original[2])
            {
                continue;
            }
            if(imagem.vermelho[linha_atual][coluna_atual] == cor_escolhida[0] && imagem.verde[linha_atual][coluna_atual] == cor_escolhida[1] && imagem.azul[linha_atual][coluna_atual] == cor_escolhida[2])
            {
                continue;
            }
            
            imagem.set_pixel(linha_atual, coluna_atual, cor_escolhida);

            vector<pair<int, int>> vizinhos = {
                {linha_atual + 1, coluna_atual},
                {linha_atual - 1, coluna_atual},
                {linha_atual, coluna_atual + 1},
                {linha_atual, coluna_atual - 1}
            };

            for(const auto& vizinho : vizinhos)
            {
                if(visitados.count(vizinho) == 0)
                {
                    pilha.push(vizinho);
                }
            }
        }

        imagem.printar("imagens/novata.ppm");

    }
};

class CarimboMaligno
{
    public:
        static void carimbar_malignamente(Imagem* imagem, Imagem* carimbo, int caracter, const vector<int>& rgb)
        {
            int meio_carimbo = carimbo->altura/2;
            int contador = 0;
            int coluna = 0;
            int coluna_origem = 0;
            
            while(contador != caracter)
            {
                int flag = 0;
                for(coluna = 0; coluna < carimbo->largura; coluna++)
                {
                    if(flag == 0 && eh_preto(carimbo, meio_carimbo, coluna))
                    {
                        //cout << "aventura";
                        flag++;
                        coluna_origem = coluna;
                    }
                    else if (flag == 1 && eh_branco(carimbo, meio_carimbo, coluna))
                    {
                        for(int corte_vertical = meio_carimbo; corte_vertical < carimbo->altura; corte_vertical++)
                        {
                            if(eh_preto(carimbo, corte_vertical, coluna))
                            {
                                flag--;
                                break;
                            }
                        }
                        flag++;
                    }
                    else if(flag == 2) 
                    {
                        contador++;
                        break;
                    }
                    
                }
            }
            imagem->carimbar(carimbo, 0, coluna_origem, coluna);
            imagem->printar("vida.ppm");
        }
    private:
        static bool eh_preto(Imagem* imagem, int linha, int coluna)
        {
            if(imagem->vermelho[linha][coluna] == 0 &&
            imagem->verde[linha][coluna] == 0 &&
            imagem->azul[linha][coluna] == 0)
            {
                return true;
            } return false;
        }
        static bool eh_branco(Imagem* imagem, int linha, int coluna)
        {
            if(imagem->vermelho[linha][coluna] == 255 &&
                imagem->verde[linha][coluna] == 255 &&
                imagem->azul[linha][coluna] == 255)
            {
                return true;
            } 
            return false;
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

void atividade_floodfill()
{
    Imagem carinhaReborn = Imagem("imagens/carinhaReborn.ppm");
    PaintBucket::floodfill(carinhaReborn, 0, 0);
}

void carimbo_malignar()
{
    Imagem carimboEvil = Imagem("imagens/impactfont.ppm");
    Imagem wilson = Imagem("imagens/mata.ppm");

    CarimboMaligno::carimbar_malignamente(&wilson, &carimboEvil, 2, {0, 0, 0});
}

int main()
{
    carimbo_malignar();
    return 0;
}