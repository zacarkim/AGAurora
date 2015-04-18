#include <opencv2/opencv.hpp>
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgcodecs.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace cv;
using namespace std;

// Métodos para Imagens ...
Mat transformaImagem(float scala, float rotacao);
Mat autocrop(Mat srcc);
Mat limpa(Mat src_);


// Métodos para Algoritmo Genético ...
void algoritmoGenetico();

float correlacaoCruzada(int povo);
int analizaValidadeCromossomo(string cromo);
int bin_to_dec(string bin);
string dec_to_bin(int bin, int range);
float transformaNumeroScalaFloat(float scala);
float transformaNumeroScalaInt(float scala);
void mediaImagens(string url);

void iniciaPopulacao(int num);
void fazSelecaoRoleta();
void fazSelecaoTorneio();
void fazSelecaoSorteio();
void fazCrossoverGene();
void fazCrossoverUmPonto();
void fazCrossoverDoisPonto();
void fazCrossoverUniforme();
void fazMutacaoGene();
void fazMutacaoBitFlip();
void fazMutacaoSwap();
//void fazMutacaoSwapBinario(); // criar!!!!!

// Variaveis Globais para Imagens ...
Mat imagemPadrao;
Mat imagemBase;
Mat imagemMediaOlho;
const int imagem_base_largura = 92;
const int imagem_base_altura = 112;

// Variaveis Globais para Algoritmo Genético ...
const int numeroPopulacao = 100;
const int maxRodadas = 50;
const int numElite = 1;
const int numChaveTorneio = 3;
const float corte = 0.7;
const float prob_crossover = 0.7;
const float prob_mutacao = 0.01;
float cromossomo[4];
vector<vector <float> > populacao;
vector<vector <float> > elite;
float ordenPopulacaoFitness[numeroPopulacao][2];
void ordenaVetorFitnessAscendente();

// Outras Variaveis ...
//ofstream file;
//string texto;


int main( int argc, char** argv )
{
    //file.open("../testessss.txt", ios_base::trunc);


    //texto += "################ ENSAIO #################\n";
    //texto += format("\nNumero de População = %d", numeroPopulacao);
    //texto += format("\nNumero de Rodadas = %d", maxRodadas);
    //texto += format("\nLimiar de Fitness = %.2f", corte);
   // texto += format("\nProbabilidade de Crossover = %.2f", prob_crossover);
   // texto += format("\nProbabilidade de Mutação = %.2f", prob_mutacao);
    //texto += "\n\n ----------------- INICIO ------------------";
   // texto += "\n\n";

    //file.open("../testessss.txt", ios_base::trunc);
	//file << texto;
	//file.close();


	imagemPadrao = imread( argv[1], 0);
	//imagemPadrao = limpa(imagemPadrao);

	string url = string(argv[2]);

//	string url2 = string(argv[3]); // para fazer média de olhos
//	mediaImagens(url2); // para fazer média de olhos


	std::ifstream arquivo(url.c_str(), ifstream::in);
	if (!arquivo) {
		string mensagemErro = "erro no nome do arquivo!.";
		cout << mensagemErro << endl;;
	}
	string urlImagem;
	while (getline(arquivo, urlImagem)) {
		//texto += " <<<<<<<< Imagem Testada = " + urlImagem;
		if(!urlImagem.empty()) {
			imagemBase = imread(urlImagem, 0);
			if (imagemBase.rows != 112 || imagemBase.cols != 92) {
				Size size(92,112);
				resize(imagemBase, imagemBase, size);
			}
			//imshow("face", imagemBase);
			//imshow("olho", imagemPadrao);

			algoritmoGenetico();

			cout << "melhorCromossomo -> " << ordenPopulacaoFitness[numeroPopulacao - 1][0] << endl;
			cout << "fitness do melhor -> " << ordenPopulacaoFitness[numeroPopulacao - 1][1] << endl;
		}
	}
	waitKey(0);
	return 0;
}

void algoritmoGenetico()
{
	Mat mostra;
	namedWindow( "Padrão", WINDOW_NORMAL ); //WINDOW_NORMAL
	namedWindow( "Base", WINDOW_NORMAL );

    // inicializa a população...

    iniciaPopulacao(numeroPopulacao); // testado;;;;;;;;;;;;;;;;;;;;;;;!

    // calcula fitness de cada cromossomo...
    int parada = 0;
    int loop = 0;

    while (parada == 0)
    {
		cout << "\n\n ===== rodada " << loop << endl;;

		for (int a = 0; a < numeroPopulacao; a++) // insere valores de fitness
		{

			float fit = correlacaoCruzada(a); // testado;;;;;;;;;;;;;;;;;;;;;;;!

			ordenPopulacaoFitness[a][0] = float(a);
			ordenPopulacaoFitness[a][1] = fit;


			cout << "Pessoa " << ordenPopulacaoFitness[a][0] << " -> Fitness = " <<
					ordenPopulacaoFitness[a][1] <<
					"  ------ Cromossomo = " << populacao[int(ordenPopulacaoFitness[a][0])][0] <<
					" | " << populacao[int(ordenPopulacaoFitness[a][0])][1] <<
					" | " << populacao[int(ordenPopulacaoFitness[a][0])][2] <<
					" | " << populacao[int(ordenPopulacaoFitness[a][0])][3] << endl;

			//texto += format("Pessoa %d -> Fitness = %d", ordenPopulacaoFitness[a][0], ordenPopulacaoFitness[a][1]);
//			mostra = imagemBase.clone();
//			Mat trans = transformaImagem(populacao[a][2], populacao[a][3] - 32);
//		    Mat pad = autocrop(trans);
//		    imshow( "Padrão", pad );
//		    Rect win(int(populacao[a][0]), int(populacao[a][1]), pad.cols, pad.rows);
//		    rectangle(mostra, win, Scalar(255,255,255), 1, 8 );
//		    imshow( "Base", mostra );
//		    waitKey(200);
		}
		//waitKey(0);
		//exit(0);

		if (elite.size() != 0)
		{
			ordenaVetorFitnessAscendente();
			for (int g = 0; g < elite.size(); g++)
			{
				populacao[int(ordenPopulacaoFitness[g][0])] = elite[g];
				float fit = correlacaoCruzada(int(ordenPopulacaoFitness[g][0])); // testado;;;;;;;;;;;;;;;;;;;;;;;!
				ordenPopulacaoFitness[g][1] = fit;

				cout << "POPULAÇÃO COM ELITE  " << ordenPopulacaoFitness[g][0] << " -> Fitness = " <<
									ordenPopulacaoFitness[g][1] <<
									"  ------ Cromossomo = " << populacao[int(ordenPopulacaoFitness[g][0])][0] <<
									" | " << populacao[int(ordenPopulacaoFitness[g][0])][1] <<
									" | " << populacao[int(ordenPopulacaoFitness[g][0])][2] <<
									" | " << populacao[int(ordenPopulacaoFitness[g][0])][3] << endl << endl;
			}
		}
		elite.clear();

		ordenaVetorFitnessAscendente();

		for (int g = numeroPopulacao; g > numeroPopulacao - numElite; g--)
		{
			vector<float> aux;
			aux.push_back(populacao[int(ordenPopulacaoFitness[g - 1][0])][0]);
			aux.push_back(populacao[int(ordenPopulacaoFitness[g - 1][0])][1]);
			aux.push_back(populacao[int(ordenPopulacaoFitness[g - 1][0])][2]);
			aux.push_back(populacao[int(ordenPopulacaoFitness[g - 1][0])][3]);
			elite.push_back(aux);

			cout << " ELITE  " << ordenPopulacaoFitness[g - 1][0] << " -> Fitness = " <<
			ordenPopulacaoFitness[g - 1][1] << " -> cromossomo = " << aux[0] << " " << aux[1] << " " <<
			aux[2] << " " << aux[3] << endl;
		}
		ordenaVetorFitnessAscendente();


		mostra = imagemBase.clone();
		Mat pad = transformaImagem(populacao[int(ordenPopulacaoFitness[numeroPopulacao - 1][0])][2],
				populacao[int(ordenPopulacaoFitness[numeroPopulacao - 1][0])][3]);
		//Mat pad = autocrop(trans);
		imshow( "Padrão", pad );
		Rect win(int(populacao[int(ordenPopulacaoFitness[numeroPopulacao - 1][0])][0]),
				 int(populacao[int(ordenPopulacaoFitness[numeroPopulacao - 1][0])][1]), pad.cols, pad.rows);
		rectangle(mostra, win, Scalar(255,255,255), 1, 8 );
		imshow( "Base", mostra );
		waitKey(1);

		cout << "Maximo Fitness = " << ordenPopulacaoFitness[numeroPopulacao - 1][1] << endl;
		//texto += format("\nMaximo Fitness = %d\n\n", max);
		loop++;

		if (ordenPopulacaoFitness[numeroPopulacao - 1][1] >= corte || maxRodadas == loop)
		{
			cout << " <<<<<<< PARADA POR ATINGIR O CORTE OU MAXIMO DE RODADAS >>>>>>>>> " << endl;
			//texto += format("\n\n<<<<<<< PARADA POR ATINGIR O CORTE OU MAXIMO DE RODADAS >>>>>>>>>\n\n");
			parada = 1;
		}
		else
		{
			fazSelecaoRoleta(); //
			//fazSelecaoTorneio();
			//fazSelecaoSorteio();

			//fazCrossoverGene(); //
			fazCrossoverUmPonto();
			//fazCrossoverDoisPonto();
			//fazCrossoverUniforme();

			//fazMutacaoGene(); //
			fazMutacaoBitFlip();
			//fazMutacaoSwap();

		}
	}
}

void ordenaVetorFitnessAscendente() //Testadooooooooooooooooooooooooooooooo
{
	int controle = 0;
	int roda = 0;
	while (controle == 0)
	{
		roda = 0;
		for (int x = 1; x < numeroPopulacao; x++)
		{
			if (ordenPopulacaoFitness[x - 1][1] > ordenPopulacaoFitness[x][1])
			{
				float aux1 = ordenPopulacaoFitness[x][0];
				float aux2 = ordenPopulacaoFitness[x][1];
				ordenPopulacaoFitness[x][0] = ordenPopulacaoFitness[x - 1][0];
				ordenPopulacaoFitness[x][1] = ordenPopulacaoFitness[x - 1][1];
				ordenPopulacaoFitness[x - 1][0] = aux1;
				ordenPopulacaoFitness[x - 1][1] = aux2;
				roda = 1;
			}
		}
		if (roda == 0)
		{
			controle = 1;
		}
	}
}

void fazSelecaoRoleta() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > copias;
	float probSelecao;
	float somatorioFitness = 0;
	float pizzaProbabilidade = 0;
	float pizzaProbabilidadeVetor[numeroPopulacao];

	srand(time(NULL));

	for (int x = 0; x < numeroPopulacao; x++)
	{
		somatorioFitness += (ordenPopulacaoFitness[x][1] + 1) / 2;
		//cout << ordenPopulacaoFitness[x][1] << " -> ordenPopulacaoFitness[x][1] " << endl;
	}
	//cout << somatorioFitness << " -> fitness " << endl;
	for (int x = 0; x < numeroPopulacao; x++)
	{

		probSelecao = (((ordenPopulacaoFitness[x][1] + 1) / 2) / somatorioFitness) * 100;
		//cout << ordenPopulacaoFitness[x][0] << " -> probSelecao -> " << probSelecao << endl;
		//cout << ordenPopulacaoFitness[x][0] << " -> fitness -> " << ordenPopulacaoFitness[x][1] << endl;

		float quantidadeCopias =  probSelecao * numeroPopulacao;
		//cout << "quantidadeCopias Prob * n_população -- -> " << quantidadeCopias << endl;
		pizzaProbabilidade += quantidadeCopias;
		//cout << "pizzaProbabilidade SOMA -- -> " << pizzaProbabilidade << endl;
		pizzaProbabilidadeVetor[x] = pizzaProbabilidade;
	}
	//cout << "Tamanho da população ANtes-- -> " << populacao.size() << endl;
	cout << "\n\n----------- Cromossomos Escolhidos por Roleta ------------" << endl;
	//texto += "\n ----------- Cromossomos Escolhidos ------------";
	for (int x = 0; x < numeroPopulacao; x++)
	{
		int escolheCromossomoProb = rand() % int(pizzaProbabilidade);
		//cout << "escolheCromossomoProb-- -> " << escolheCromossomoProb <<
		//" | pizzaProbabilidade-- -> " << int(pizzaProbabilidade) << endl;
		for (int y = 0; y < numeroPopulacao; y++)
		{
			if (float(escolheCromossomoProb) < pizzaProbabilidadeVetor[y])
			{
				copias.push_back(populacao[int(ordenPopulacaoFitness[y][0])]);
				cout << "\nCromossomo  " << ordenPopulacaoFitness[y][0] << " -> Fitness " << ordenPopulacaoFitness[y][1];
				//texto += format("\nCromossomo %d  -> Fitness %d", ordenPopulacaoFitness[y][0], ordenPopulacaoFitness[y][1]);
				break;
			}
		}
	}

	populacao.clear();
	populacao = copias;
	//cout << "Tamanho da população -> " << populacao.size() << endl;
}

void fazSelecaoTorneio() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > copias;
	int ganhadorTorneio;

	srand(time(NULL));

	for (int x = 0; x < numeroPopulacao; x++)
	{
		ganhadorTorneio = ordenPopulacaoFitness[x][0];
		for (int d = 0; d < numChaveTorneio; d++)
		{
			int num = rand() % numeroPopulacao;
			if (ganhadorTorneio <= ordenPopulacaoFitness[num][1])
			{
				ganhadorTorneio = ordenPopulacaoFitness[num][0];
			}
		}
		copias.push_back(populacao[int(ordenPopulacaoFitness[ganhadorTorneio][0])]);
	}
	populacao.clear();
	populacao = copias;
	//cout << "Tamanho da população -> " << populacao.size() << endl;
}

void fazSelecaoSorteio() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > copias;

	srand(time(NULL));

	for (int x = 0; x < numeroPopulacao; x++)
	{
		int num = rand() % numeroPopulacao;
		copias.push_back(populacao[int(ordenPopulacaoFitness[num][0])]);
	}
	populacao.clear();
	populacao = copias;
	//cout << "Tamanho da população -> " << populacao.size() << endl;
}

void fazCrossoverGene() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > par;
	vector<vector <float> > copia;
	copia = populacao;
	//cout << " --- populacao Inicio = " << populacao.size() << endl;

	srand(time(NULL));
	int w = numeroPopulacao;
	//cout << " -----------ERRO 01 ------------- " << endl;
	for (int a = 0; a < numeroPopulacao / 2; a++)
	{
		int escolha = rand() % w; // sorteia um cromossomo para parear
		//cout << " -----------ERRO interno loop = " << a <<
		// " --- escolha rand = " << escolha << " --- copia size = " << copia.size() <<
		// " --- par = " << par.size() <<
		//  " --- populacao = " << populacao.size() << endl;
		par.push_back(copia[escolha]);
		copia.erase(copia.begin() + escolha);
		w--;
	}
	//cout << " -----------ERRO 02 ------------- " << endl;

	//for (int b = 0; b < par.size(); b++)
	//{
		//cout << "\n----------- GENES PAREADOS ---------------\n";
		//cout << " cromossomo1 par ->  " << par[b][0] << " ; " << par[b][1] << " ; " << par[b][2] << " ; " << par[b][3];
		//cout << " |||| cromossomo2 copia->  " << copia[b][0] << " ; " << copia[b][1] << " ; " << copia[b][2] << " ; " << copia[b][3] << endl;

	//}
	cout << "\n\n----------- Crossover ------------";
	//file << "\n\n----------- Crossover ------------";
	for (int b = 0; b < numeroPopulacao / 2; b++)
	{
		float escolha = rand() % 101; // sorteia a probabilidade de crossover do par
		escolha = escolha  / 100;
		if (escolha <= prob_crossover)
		{
			//cout << "\n Gene 1 antes ->  " << par[b][0] << " ; " << par[b][1] << " ; " << par[b][2] << " ; " << par[b][3];
			//cout << "\n Gene 2 antes->  " << copia[b][0] << " ; " << copia[b][1] << " ; " << copia[b][2] << " ; " << copia[b][3];

			int escolhaPontoCorte = rand() % 4; // sorteia o ponto de corte do gem para crossover
			float aux = par[b][escolhaPontoCorte];
			par[b][escolhaPontoCorte] = copia[b][escolhaPontoCorte];
			copia[b][escolhaPontoCorte] = aux;
			//cout << "\n Gene 1 depois ->  " << par[b][0] << " ; " << par[b][1] << " ; " << par[b][2] << " ; " << par[b][3];
			//cout << "\n Gene 2 depois->  " << copia[b][0] << " ; " << copia[b][1] << " ; " << copia[b][2] << " ; " << copia[b][3] << endl;

		}
	}
	populacao.clear();
	for (int k = 0; k < par.size(); k++)
	{
		populacao.push_back(par[k]);
		populacao.push_back(copia[k]);
	}
}

int bin_to_dec(string bin) //Testadooooooooooooooooooooooooooooooo
{
	int soma = 0;
	int potenc = 1;

	for (int x = bin.length() - 1; x >= 0; x--)
	{
		char pos = bin[x];
		if (pos == '1')
		{
			soma += potenc;
		}
		potenc = potenc * 2;
	}
	return soma;
}

string dec_to_bin(int bin, int range) //Testadooooooooooooooooooooooooooooooo
{
	char binario[2];
	string w, z;
	do
	{
		int a = bin % 2;
	    sprintf(binario, "%d", a);
	    w.push_back(binario[0]);
	    bin = bin / 2;
	}
	while(bin >= 1);

	int tamW = w.length();

	for (int v = 0; v < range - tamW; v++)
	{
		 w.push_back('0');
	}

	for (int v = w.length() - 1; v >= 0; v--)
	{
		z.push_back(w[v]);
	}

	return z;
}

int analizaValidadeCromossomo(string cromo) //Testadooooooooooooooooooooooooooooooo
{
	string g0 = cromo.substr(0, 7);
	string g1 = cromo.substr(7, 7);
	string g2 = cromo.substr(14, 3);
	string g3 = cromo.substr(17, 6);

	int cr0 = bin_to_dec(g0);
	int cr1 = bin_to_dec(g1);
	int cr2 = bin_to_dec(g2);
	int cr3 = bin_to_dec(g3);

	//if (cr0 >= 0 && cr0 <= 92 - 49 && cr1 >= 0 && cr1 <= 112 - 40 && cr2 >= 0 && cr2 <= 7 && cr3 >= 0 && cr3 <= 63)
	if (cr0 >= 0 && cr0 <= 92 - imagemPadrao.cols && cr1 >= 0 && cr1 <= 112 - imagemPadrao.rows && cr2 >= 0 && cr2 <= 7 && cr3 >= 0 && cr3 <= 63)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void fazCrossoverUmPonto() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > par;
	vector<vector <float> > copia;
	vector<vector <float> > filhos;
	copia = populacao;

	srand(time(NULL));
	int w = numeroPopulacao;
	for (int a = 0; a < numeroPopulacao / 2; a++)
	{
		int escolha = rand() % w; // sorteia um cromossomo para parear

		par.push_back(copia[escolha]);
		copia.erase(copia.begin() + escolha);
		w--;
	}

	cout << "\n\n----------- Crossover Um Ponto ------------\n";
	//file << "\n\n----------- Crossover ------------";
	for (int b = 0; b < numeroPopulacao / 2; b++)
	{
		float escolha = rand() % 101; // sorteia a probabilidade de crossover do par
		escolha = escolha  / 100;
		if (escolha <= prob_crossover)
		{
			string pai1 = "";
			string BitCromo1 = dec_to_bin(int(par[b][0]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(par[b][1]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(transformaNumeroScalaInt(par[b][2])), 3);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(par[b][3]), 6);
			pai1 += BitCromo1;

//			cout << "Pai 1 = " << par[b][0] << " " << par[b][1] << " " << par[b][2] << " " << par[b][3] << endl;
//			cout << "Cromossomo Binario Pai 1 = " << pai1 << endl;

			string pai2 = "";
			string BitCromo2 = dec_to_bin(int(copia[b][0]), 7);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(copia[b][1]), 7);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(transformaNumeroScalaInt(copia[b][2])), 3);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(copia[b][3]), 6);
			pai2 += BitCromo2;

//			cout << "Pai 2 = " << copia[b][0] << " " << copia[b][1] << " " << copia[b][2] << " " << copia[b][3] << endl;
//			cout << "Cromossomo Binario Pai 2 = " << pai2 << endl;

			string copiaPai1 = pai1;
			string copiaPai2 = pai2;

			int controle = 0;
			while (controle == 0)
			{
				pai1 = copiaPai1;
				pai2 = copiaPai2;

				int escolhaPontoCorte = (rand() % 22) + 1; // sorteia o ponto de corte do cromossomo para crossover
				string subPai1 = pai1.substr(escolhaPontoCorte, pai1.length() - escolhaPontoCorte);
				string subPai2 = pai2.substr(escolhaPontoCorte, pai2.length() - escolhaPontoCorte);
				pai1.replace(escolhaPontoCorte, pai1.length() - escolhaPontoCorte, subPai2);
				pai2.replace(escolhaPontoCorte, pai2.length() - escolhaPontoCorte, subPai1);

				if (analizaValidadeCromossomo(pai1) == 1 && analizaValidadeCromossomo(pai2) == 1)
				{
					controle = 1;
				}
//				cout << "Ponto de Corte = " << escolhaPontoCorte << endl;
//				cout << "Cromossomo Binario com crossover Pai 1 = " << pai1 << endl;
//				cout << "Cromossomo Binario com crossover Pai 2 = " << pai2 << endl;
			}

//			cout << "Cromossomo Binario com crossover Pai 1 = " << pai1 << endl;
//			cout << "Cromossomo Binario com crossover Pai 2 = " << pai2 << endl;

			vector<float> aux;
			aux.push_back(float(bin_to_dec(pai1.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai1.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai1.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai1.substr(17, 6))));

//			cout << "Filhos 1 = " << aux[0] << " " << aux[1] << " " << aux[2] << " " << aux[3] << endl;

			filhos.push_back(aux);

			aux.clear();
			aux.push_back(float(bin_to_dec(pai2.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai2.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai2.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai2.substr(17, 6))));

//			cout << "Filhos 2 = " << aux[0] << " " << aux[1] << " " << aux[2] << " " << aux[3] << endl;

			filhos.push_back(aux);

//			cout << "Apagando Pai 1 = " << par[b][0] << " " << par[b][1] << " " << par[b][2] << " " << par[b][3] << endl;
//			cout << "Apagando Pai 2 = " << copia[b][0] << " " << copia[b][1] << " " << copia[b][2] << " " << copia[b][3] << endl << endl;
			copia.erase(copia.begin() + b);
			par.erase(par.begin() + b);
		}
	}
//	cout << "pai 1 resto = " << par.size() << endl;
//	cout << "pai 2 resto = " << copia.size() << endl;

	for (int k = 0; k < par.size(); k++)
	{
		filhos.push_back(par[k]);
		filhos.push_back(copia[k]);
	}
	populacao.clear();
	populacao = filhos;
}

void fazCrossoverDoisPonto() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > par;
	vector<vector <float> > copia;
	vector<vector <float> > filhos;
	copia = populacao;

	srand(time(NULL));
	int w = numeroPopulacao;
	for (int a = 0; a < numeroPopulacao / 2; a++)
	{
		int escolha = rand() % w; // sorteia um cromossomo para parear

		par.push_back(copia[escolha]);
		copia.erase(copia.begin() + escolha);
		w--;
	}

	cout << "\n\n----------- Crossover Dois Pontos ------------\n";
	for (int b = 0; b < numeroPopulacao / 2; b++)
	{
		float escolha = rand() % 101; // sorteia a probabilidade de crossover do par
		escolha = escolha  / 100;
		if (escolha <= prob_crossover)
		{
			string pai1 = "";
			string BitCromo1 = dec_to_bin(int(par[b][0]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(par[b][1]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(transformaNumeroScalaInt(par[b][2])), 3);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(par[b][3]), 6);
			pai1 += BitCromo1;

			string pai2 = "";
			string BitCromo2 = dec_to_bin(int(copia[b][0]), 7);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(copia[b][1]), 7);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(transformaNumeroScalaInt(copia[b][2])), 3);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(copia[b][3]), 6);
			pai2 += BitCromo2;

			//cout << "Cromossomo Binario Pai 1 = " << pai1 << endl;
			//cout << "Cromossomo Binario Pai 2 = " << pai2 << endl;

			string copiaPai1 = pai1;
			string copiaPai2 = pai2;

			int controle = 0;
			while (controle == 0)
			{
				pai1 = copiaPai1;
				pai2 = copiaPai2;

				int escolhaPontoCorte1 = (rand() % 22) + 1; // sorteia o ponto de corte do cromossomo para crossover
				int escolhaPontoCorte2 = (rand() % 22) + 1; // sorteia o ponto de corte do cromossomo para crossover

				if (escolhaPontoCorte1 < escolhaPontoCorte2)
				{
					string subPai1 = pai1.substr(escolhaPontoCorte1, escolhaPontoCorte2 - escolhaPontoCorte1);
					string subPai2 = pai2.substr(escolhaPontoCorte1, escolhaPontoCorte2 - escolhaPontoCorte1);
					pai1.replace(escolhaPontoCorte1, escolhaPontoCorte2 - escolhaPontoCorte1, subPai2);
					pai2.replace(escolhaPontoCorte1, escolhaPontoCorte2 - escolhaPontoCorte1, subPai1);
				}
				else if (escolhaPontoCorte1 > escolhaPontoCorte2)
				{
					string subPai1 = pai1.substr(escolhaPontoCorte2, escolhaPontoCorte1 - escolhaPontoCorte2);
					string subPai2 = pai2.substr(escolhaPontoCorte2, escolhaPontoCorte1 - escolhaPontoCorte2);
					pai1.replace(escolhaPontoCorte2, escolhaPontoCorte1 - escolhaPontoCorte2, subPai2);
					pai2.replace(escolhaPontoCorte2, escolhaPontoCorte1 - escolhaPontoCorte2, subPai1);
				}
				else
				{
					continue;
				}

				if (analizaValidadeCromossomo(pai1) == 1 && analizaValidadeCromossomo(pai2) == 1)
				{
					controle = 1;
				}

				//cout << "Ponto de Corte 1 = " << escolhaPontoCorte1 << " Ponto de Corte 2 = " << escolhaPontoCorte2 << endl;
				//cout << "Cromossomo Binario com crossover Pai 1 = " << pai1 << endl;
				//cout << "Cromossomo Binario com crossover Pai 2 = " << pai2 << endl;
			}

			vector<float> aux;
			aux.push_back(float(bin_to_dec(pai1.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai1.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai1.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai1.substr(17, 6))));

			filhos.push_back(aux);

			aux.clear();
			aux.push_back(float(bin_to_dec(pai2.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai2.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai2.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai2.substr(17, 6))));

			filhos.push_back(aux);

			copia.erase(copia.begin() + b);
			par.erase(par.begin() + b);
		}
	}
	for (int k = 0; k < par.size(); k++)
	{
		filhos.push_back(par[k]);
		filhos.push_back(copia[k]);
	}
	populacao.clear();
	populacao = filhos;
}

void fazCrossoverUniforme() //Testadooooooooooooooooooooooooooooooo
{
	vector<vector <float> > par;
	vector<vector <float> > copia;
	vector<vector <float> > filhos;
	copia = populacao;

	srand(time(NULL));
	int w = numeroPopulacao;
	for (int a = 0; a < numeroPopulacao / 2; a++)
	{
		int escolha = rand() % w; // sorteia um cromossomo para parear

		par.push_back(copia[escolha]);
		copia.erase(copia.begin() + escolha);
		w--;
	}

	cout << "\n\n----------- Crossover Uniforme ------------\n";
	for (int b = 0; b < numeroPopulacao / 2; b++)
	{
		float escolha = rand() % 101; // sorteia a probabilidade de crossover do par
		escolha = escolha  / 100;
		if (escolha <= prob_crossover)
		{
			string pai1 = "";
			string BitCromo1 = dec_to_bin(int(par[b][0]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(par[b][1]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(transformaNumeroScalaInt(par[b][2])), 3);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(par[b][3]), 6);
			pai1 += BitCromo1;

			string pai2 = "";
			string BitCromo2 = dec_to_bin(int(copia[b][0]), 7);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(copia[b][1]), 7);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(transformaNumeroScalaInt(copia[b][2])), 3);
			pai2 += BitCromo2;
			BitCromo2 = dec_to_bin(int(copia[b][3]), 6);
			pai2 += BitCromo2;

			//cout << "Cromossomo Binario Pai 1 = " << pai1 << endl;
			//cout << "Cromossomo Binario Pai 2 = " << pai2 << endl;

			string copiaPai1 = pai1;
			string copiaPai2 = pai2;

			int controle = 0;
			while (controle == 0)
			{
				pai1 = copiaPai1;
				pai2 = copiaPai2;

				for (int z = 0; z < copiaPai1.length(); z++)
				{
					float escolha2 = rand() % 101; // sorteia a probabilidade de crossover do par
					escolha2 = escolha2  / 100;
					if (escolha2 <= prob_crossover)
					{
						char troca = pai1[z];
						pai1[z] = pai2[z];
						pai2[z] = troca;

						//cout << "Troca Pai1 Ponto = " << z << " Por Pai2 Ponto = " << z << endl;
					}
				}
				if (analizaValidadeCromossomo(pai1) == 1 && analizaValidadeCromossomo(pai2) == 1)
				{
					controle = 1;
				}

				//cout << "Cromossomo Binario com crossover Pai 1 = " << pai1 << endl;
				//cout << "Cromossomo Binario com crossover Pai 2 = " << pai2 << endl;

			}

			vector<float> aux;
			aux.push_back(float(bin_to_dec(pai1.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai1.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai1.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai1.substr(17, 6))));

			filhos.push_back(aux);

			aux.clear();
			aux.push_back(float(bin_to_dec(pai2.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai2.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai2.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai2.substr(17, 6))));

			filhos.push_back(aux);

			copia.erase(copia.begin() + b);
			par.erase(par.begin() + b);
		}
	}
	for (int k = 0; k < par.size(); k++)
	{
		filhos.push_back(par[k]);
		filhos.push_back(copia[k]);
	}
	populacao.clear();
	populacao = filhos;
}

void fazMutacaoGene() //Testadooooooooooooooooooooooooooooooo
{
	Mat a, b;
	srand(time(NULL));
	cout << "\n----------- Mutação Gene ------------\n";
	//file << "\n\n----------- Mutação ------------";
	for (int k = 0; k < numeroPopulacao; k++)
	{
		float escolhaProbMutacao = rand() % 101; // sorteia a probabilidade de mutação
		escolhaProbMutacao = escolhaProbMutacao / 100;
		if (escolhaProbMutacao <= prob_mutacao)
		{
			cout << "\nGene antes ->  " << populacao[k][0] << " ; "
			 << populacao[k][1] << " ; " << populacao[k][2] << " ; " << populacao[k][3];
			//file << "\nGene antes ->  " << populacao[k][0] << " ; "
			// << populacao[k][1] << " ; " << populacao[k][2] << " ; " << populacao[k][3];


			int escolhaMutacaoGene = rand() % 4; // sorteia o gene para mutação
			switch (escolhaMutacaoGene)
			{
				case 0:
					b = transformaImagem(populacao[k][2], populacao[k][3]);
					//b = autocrop(a);
					cromossomo[0] = rand() % (imagem_base_largura - b.cols); //x
					populacao[k][escolhaMutacaoGene] = cromossomo[0];
					break;
				case 1:
					b = transformaImagem(populacao[k][2], populacao[k][3]);
					//b = autocrop(a);
					cromossomo[1] = rand() % (imagem_base_altura - b.rows);//y
					populacao[k][escolhaMutacaoGene] = cromossomo[1];
					break;
				case 2:
					switch (rand() % 8)
					{
						case 0:
							populacao[k][escolhaMutacaoGene] = 0.5; // scala
							break;
						case 1:
							populacao[k][escolhaMutacaoGene] = 0.75;
							break;
						case 2:
							populacao[k][escolhaMutacaoGene] = 1;
							break;
						case 3:
							populacao[k][escolhaMutacaoGene] = 1.25;
							break;
						case 4:
							populacao[k][escolhaMutacaoGene] = 1.5;
							break;
						case 5:
							populacao[k][escolhaMutacaoGene] = 1.75;
							break;
						case 6:
							populacao[k][escolhaMutacaoGene] = 2;
							break;
						case 7:
							populacao[k][escolhaMutacaoGene] = 2.25;
							break;
					}
					break;
				case 3:
					populacao[k][escolhaMutacaoGene] = rand() % 64;
					break;
			}
			cout << "\nGene depois ->  " << populacao[k][0] << " ; "
			 << populacao[k][1] << " ; " << populacao[k][2] << " ; " << populacao[k][3];
			//file << "\nGene depois ->  " << populacao[k][0] << " ; "
			 //<< populacao[k][1] << " ; " << populacao[k][2] << " ; " << populacao[k][3];
		}
	}
}

float transformaNumeroScalaFloat(float scala) //Testadooooooooooooooooooooooooooooooo
{
	switch (int(scala))
	{
		case 0:
			return 0.5; // scala
			break;
		case 1:
			return 0.75;
			break;
		case 2:
			return 1;
			break;
		case 3:
			return 1.25;
			break;
		case 4:
			return 1.5;
			break;
		case 5:
			return 1.75;
			break;
		case 6:
			return 2;
			break;
		case 7:
			return 2.25;
			break;
	}
}

float transformaNumeroScalaInt(float scala) //Testadooooooooooooooooooooooooooooooo
{
	float num;
	if (scala == 0.5)
	{
		num = 0;
	}
	else if (scala == 0.75)
	{
		num = 1;
	}
	else if (scala == 1)
	{
		num = 2;
	}
	else if (scala == 1.25)
	{
		num = 3;
	}
	else if (scala == 1.5)
	{
		num = 4;
	}
	else if (scala == 1.75)
	{
		num = 5;
	}
	else if (scala == 2)
	{
		num = 6;
	}
	else if (scala == 2.25)
	{
		num = 7;
	}
	return num;
}

void fazMutacaoBitFlip() //Testadooooooooooooooooooooooooooooooo
{

	vector<vector <float> > copia;
	vector<vector <float> > filhos;
	copia = populacao;

	srand(time(NULL));

	cout << "\n----------- Mutação BitFlip ------------\n";

	for (int b = 0; b < numeroPopulacao; b++)
	{
		float escolha = rand() % 101; // sorteia a probabilidade de crossover do par
		escolha = escolha  / 100;
		if (escolha <= prob_mutacao)
		{
			string pai1 = "";
			string BitCromo1 = dec_to_bin(int(copia[b][0]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(copia[b][1]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(transformaNumeroScalaInt(copia[b][2])), 3);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(copia[b][3]), 6);
			pai1 += BitCromo1;

			//cout << "Cromossomo Binario Pai " << b << " = " << pai1 << endl;

			string copiaPai1 = pai1;

			int controle = 0;
			while (controle == 0)
			{
				pai1 = copiaPai1;

				for (int z = 0; z < copiaPai1.length(); z++)
				{
					float escolha2 = rand() % 101; // sorteia a probabilidade de crossover do par
					escolha2 = escolha2  / 100;
					if (escolha2 <= prob_mutacao)
					{
						if (pai1[z] == '0')
						{
							pai1[z] = '1';
						}
						else if (pai1[z] == '1')
						{
							pai1[z] = '0';
						}
						//cout << "Troca Ponto = " << z << endl;
					}
				}
				if (analizaValidadeCromossomo(pai1) == 1)
				{
					controle = 1;
				}
				//cout << "Cromossomo Binario com Mutação Bitflip Pai 1 = " << pai1 << endl << endl;
			}

			vector<float> aux;
			aux.push_back(float(bin_to_dec(pai1.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai1.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai1.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai1.substr(17, 6))));

			filhos.push_back(aux);

			copia.erase(copia.begin() + b);
		}
	}
	for (int k = 0; k < copia.size(); k++)
	{
		filhos.push_back(copia[k]);
	}
	populacao.clear();
	populacao = filhos;
}

void fazMutacaoSwap() //Testadooooooooooooooooooooooooooooooo
{

	vector<vector <float> > copia;
	vector<vector <float> > filhos;
	copia = populacao;

	srand(time(NULL));

	cout << "\n----------- Mutação Swap ------------\n";

	for (int b = 0; b < numeroPopulacao; b++)
	{
		float escolha = rand() % 101; // sorteia a probabilidade de crossover do par
		escolha = escolha  / 100;
		if (escolha <= prob_mutacao)
		{
			string pai1 = "";
			string BitCromo1 = dec_to_bin(int(copia[b][0]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(copia[b][1]), 7);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(transformaNumeroScalaInt(copia[b][2])), 3);
			pai1 += BitCromo1;
			BitCromo1 = dec_to_bin(int(copia[b][3]), 6);
			pai1 += BitCromo1;

			//cout << "Cromossomo Binario Pai " << b << " = " << pai1 << endl;

			string copiaPai1 = pai1;

			int controle = 0;
			while (controle == 0)
			{
				pai1 = copiaPai1;

				for (int z = 0; z < copiaPai1.length(); z++)
				{
					float escolha2 = rand() % 101; // sorteia a probabilidade de crossover do par
					escolha2 = escolha2  / 100;
					if (escolha2 <= prob_mutacao)
					{
						int escolha3 = rand() % 23;
						char troca = pai1[z];
						pai1[z] = pai1[escolha3];
						pai1[escolha3] = troca;

						//cout << "Troca Ponto = " << z << " Por Ponto = " << escolha3 << " filho = " << pai1 <<  endl;
					}
				}
				if (analizaValidadeCromossomo(pai1) == 1)
				{
					controle = 1;
				}
				//cout << "Cromossomo Binario com Mutação Bitflip Pai 1 = " << pai1 << endl << endl;
			}

			vector<float> aux;
			aux.push_back(float(bin_to_dec(pai1.substr(0, 7))));
			aux.push_back(float(bin_to_dec(pai1.substr(7, 7))));
			aux.push_back(transformaNumeroScalaFloat(float(bin_to_dec(pai1.substr(14, 3)))));
			aux.push_back(float(bin_to_dec(pai1.substr(17, 6))));

			filhos.push_back(aux);

			copia.erase(copia.begin() + b);
		}
	}
	for (int k = 0; k < copia.size(); k++)
	{
		filhos.push_back(copia[k]);
	}
	populacao.clear();
	populacao = filhos;
}

void iniciaPopulacao(int num) //Testadooooooooooooooooooooooooooooooo
{
	//texto += "cromossomo 1 |  cromossomo 2 | cromossomo 3 |  cromossomo 4 ";
	cout << "cromossomo 1 |  cromossomo 2 | cromossomo 3 |  cromossomo 4 " << endl;
    srand(time(NULL));
    for (int l = 0; l < num; l++)
    {
		int auxRand = rand() % 8;
		switch (auxRand)
		{
			case 0:
				cromossomo[2] = 0.5; // scala
				break;
			case 1:
				cromossomo[2] = 0.75;
				break;
			case 2:
				cromossomo[2] = 1;
				break;
			case 3:
				cromossomo[2] = 1.25;
				break;
			case 4:
				cromossomo[2] = 1.5;
				break;
			case 5:
				cromossomo[2] = 1.75;
				break;
			case 6:
				cromossomo[2] = 2;
				break;
			case 7:
				cromossomo[2] = 2.25;
				break;
		}

		cromossomo[3] = rand() % 64; //rotação

		Mat b = transformaImagem(cromossomo[2], cromossomo[3]);
		//Mat b = autocrop(a);


		cromossomo[0] = rand() % (imagem_base_largura - b.cols); //x olharrrrrrrrrrrrrrrrrr
		cromossomo[1] = rand() % (imagem_base_altura - b.rows);//y olharrrrrrrrrrrrrrrrrr

		cout << cromossomo[0] << " | " << cromossomo[1] << " | " << cromossomo[2] << " | " << cromossomo[3] << endl;;
		//texto += format("\n%d | %d | %d | %d", cromossomo[0], cromossomo[1], cromossomo[2], cromossomo[3]);

		vector<float> agregaGems;
		agregaGems.push_back(cromossomo[0]);
		agregaGems.push_back(cromossomo[1]);
		agregaGems.push_back(cromossomo[2]);
		agregaGems.push_back(cromossomo[3]);

		populacao.push_back(agregaGems);
	}
}

float correlacaoCruzada(int povo) //Testadooooooooooooooooooooooooooooooo
{
//	cout << "X povo = " << populacao[povo][0] << " Y povo = " << populacao[povo][1] << endl;
//	cout << "scala = " << populacao[povo][2] << " rotação = " << populacao[povo][3] << endl;

	Mat b = transformaImagem(populacao[povo][2], populacao[povo][3]);
	//Mat b = autocrop(a);


//	cout << "X povo = " << populacao[povo][0] << "Y povo = " << populacao[povo][1] << endl;
//	cout << "scala = " << populacao[povo][2] << "rotação = " << populacao[povo][3] - 32 << endl;
//	cout << "X = " << b.cols << "Y = " << b.rows << endl;


	//imshow("a", a);
	//imshow("b", b);
	//waitKey(30);


	double sigmaI = 0;
	double sigmaT = 0;
	double somatorioNumerador = 0;
	double somatorioDenominador1 = 0;
	double somatorioDenominador2 = 0;
	float lambda;
	double somatorioDenominador = 0;
	int I = 0;
	int I_ = 0;
	int T = 0;
	int T_ = 0;
	int cont = 0;
	int cont2 = 0;

	// calcula média.
	//Mat somas = b.clone();
	//Mat somas2 = imagemBase.clone();
	//namedWindow( "Somas", WINDOW_NORMAL ); //WINDOW_NORMAL
	//namedWindow( "Somas2", WINDOW_NORMAL ); //WINDOW_NORMAL

	for (int y = 0; y < b.rows; y++)
	{
		for (int x = 0; x < b.cols; x++)
		{
			//cout << "x = " << x << " y = " << y << endl;
			int corPixelBase = imagemBase.at<uchar>(Point(int(populacao[povo][0]) + x, int(populacao[povo][1]) + y));
			int corPixelPadrao = b.at<uchar>(Point(x,y));
			//if (corPixelPadrao != 255)
			//{
				//somas.at<uchar>(Point(x,y)) = 0;
				//imshow("Somas", somas);
				//somas2.at<uchar>(Point(int(populacao[povo][0]) + x, int(populacao[povo][1]) + y)) = 255;
				//imshow("Somas2", somas2);
				//waitKey(1);

				I += corPixelBase;
				T += corPixelPadrao;
				cont++;
			//}
			cont2++;
		}
	}
	//I_ = ((I * 1) / (cont)) * (cont / cont2);
	//T_ = ((T * 1) / (cont)) * (cont / cont2);

	I_ = (I * 1) / cont;
	T_ = (T * 1) / cont;


	// calcula coeficiente de correlação cruzada.

//	Mat mostr = b.clone();
//	Mat mostr2 = imagemBase.clone();
//	namedWindow( "Teste", WINDOW_NORMAL ); //WINDOW_NORMAL
//	namedWindow( "Teste2", WINDOW_NORMAL ); //WINDOW_NORMAL

	for (int y = 0; y < b.rows; y++)
	{
		for (int x = 0; x < b.cols; x++)
		{
			int corPixelBase = imagemBase.at<uchar>(Point(populacao[povo][0] + x, populacao[povo][1] + y));
			int corPixelPadrao = b.at<uchar>(Point(x,y));
			//if (corPixelPadrao != 255)
			//{
//				mostr.at<uchar>(Point(x,y)) = 0;
//				imshow("Teste", mostr);
//				mostr2.at<uchar>(Point(populacao[povo][0] + x, populacao[povo][1] + y)) = 255;
//				imshow("Teste2", mostr2);
//				waitKey(3);

				sigmaI = corPixelBase - I_;
				sigmaT = corPixelPadrao - T_;
				somatorioNumerador += sigmaI * sigmaT;
				//cout << "sigmaI = " << sigmaI << " sigmaT = " << sigmaT << " somatorioNumerador = " << somatorioNumerador << endl;

				somatorioDenominador1 += (sigmaI * sigmaI);
				somatorioDenominador2 += (sigmaT * sigmaT);
				//cout << "somatorioDenominador1 = " << somatorioDenominador1 << " somatorioDenominador2 = "
				// << somatorioDenominador2 << endl;

			//}
		}
	}

	//somatorioDenominador = 	somatorioDenominador1 * somatorioDenominador2;
	//cout << "somatorioDenominador Geral  1 * 2 = " << somatorioDenominador << endl;

	somatorioDenominador = 	sqrt(somatorioDenominador1 * somatorioDenominador2);
	//cout << "somatorioDenominador Geral  RAIZ (1 * 2) = " << somatorioDenominador << endl;

	lambda = somatorioNumerador / somatorioDenominador;
	//cout << "numerador = " << somatorioNumerador << " denominador = " << somatorioDenominador << endl;
//	float l = float(cont) / float(cont2);
//	cout << l << " " << cont << " " << cont2 << endl;
	return lambda;
	//return lambda * (float(cont) / float(cont2));
}

Mat autocrop(Mat srcc) //Testadooooooooooooooooooooooooooooooo
{
	int w = 49;
	int h = 40;

	Rect win3(srcc.cols / 2 - w/2, srcc.rows / 2 - h/2, w, h);
	srcc = srcc(win3);
/*
    //Mat black = Mat::zeros(srcc.size(), 0);

    int maxX[2] = {0, 0};
    int minX[2] = {0, 0};
    int maxY[2] = {0, 0};
    int minY[2] = {0, 0};

    for(int y = 0; y < srcc.rows ;y++) {
        for(int x = 0; x < srcc.cols ;x++) {
            Scalar color = srcc.at<uchar>(Point(x,y));
            if(color.val[0] != 255) {
                if (minY[0] == 0) {
                    //circle(black, Point(x,y), 100/32.0, Scalar( 255, 255, 255 ), -1, 8 );
                    minY[0] = x;
                    minY[1] = y;
                }
                maxY[0] = x;
                maxY[1] = y;
            }
        }
    }
    //circle(black, Point(maxY[0], maxY[1]), 100/32.0, Scalar( 255, 255, 255 ), -1, 8 );

    for(int x = 0; x < srcc.cols ;x++) {
        for(int y = 0; y < srcc.rows ;y++) {
            Scalar color = srcc.at<uchar>(Point(x,y));
            if(color.val[0] != 255) {
                if (minX[0] == 0) {
                    //circle(black, Point(x,y), 100/32.0, Scalar( 255, 255, 255 ), -1, 8 );
                    minX[0] = x;
                    minX[1] = y;
                }
                maxX[0] = x;
                maxX[1] = y;
            }
        }
    }
    //circle(black, Point(maxX[0], maxX[1]), 100/32.0, Scalar( 255, 255, 255 ), -1, 8 );

    //imshow( "aa", black );

    int coordInicioRetangulo_X = minY[0] - (minY[0] - minX[0]);
    int coordInicioRetangulo_Y = minX[1] - (minX[1] - minY[1]);

    int pontaComprimento_X = maxX[0];
//    int pontaComprimento_Y = maxY[1] - (maxY[1] - minY[1]);

//    int pontaLargura_X = maxY[0] - (maxY[0] - minX[0]);
    int pontaLargura_Y = maxY[1];

    int comprimentoRetangulo = pontaComprimento_X - coordInicioRetangulo_X;
    int larguraRetangulo = pontaLargura_Y - coordInicioRetangulo_Y;

    Rect win(coordInicioRetangulo_X + 1, coordInicioRetangulo_Y + 1, comprimentoRetangulo - 1, larguraRetangulo - 1);
    srcc = srcc(win);
*/
    return srcc;
}

Mat limpa(Mat src_) //Testadooooooooooooooooooooooooooooooo
{
	for(int y = 0; y < src_.rows ;y++) {
		for(int x = 0; x < src_.cols ;x++) {
			Scalar color = src_.at<uchar>(Point(x,y));
			if(color.val[0] == 0) {
				src_.at<uchar>(Point(x,y)) = 1;
			}
			if(color.val[0] == 255) {
				src_.at<uchar>(Point(x,y)) = 254;
			}
		}
	}
	//imshow("sss", src_);
	Mat teste = Mat::zeros(Size(300, 300), 0);

	for(int y = 0; y < teste.rows ;y++) {
		for(int x = 0; x < teste.cols ;x++) {
			teste.at<uchar>(Point(x,y)) = 255;
		}
	}

	Mat f;
	Rect win(0, 0, src_.cols, src_.rows);
	f = imagemPadrao(win);
	Rect win2(150 - (src_.cols/2), 150 - (src_.rows/2), src_.cols, src_.rows);
	f.copyTo(teste(win2));
	return teste;
}

Mat transformaImagem(float scala, float rotacao) //Testadooooooooooooooooooooooooooooooo
{
	Mat imagemResultado = imagemPadrao.clone();
    Point centroImagem = Point( imagemResultado.cols/2, imagemResultado.rows/2 );
    Mat rot_mat = getRotationMatrix2D( centroImagem, double(rotacao - 32), double(scala));
    warpAffine( imagemResultado, imagemResultado, rot_mat, imagemResultado.size() );
    return imagemResultado;
}

void mediaImagens(string url)
{
	vector<Mat> olhos;
	std::ifstream arquivo(url.c_str(), ifstream::in);
	if (!arquivo)
	{
		string mensagemErro = "erro no nome do arquivo!.";
		cout << mensagemErro << endl;
	}
	string urlImagem;
	while (getline(arquivo, urlImagem))
	{
		//cout << " <<<<<<<< Imagem Testada = " << urlImagem << endl;
		if(!urlImagem.empty())
		{
			Mat olho = imread(urlImagem, 0);
			if (olho.cols != 25 || olho.rows != 12)
			{
				Size size(25,12);
				resize(olho, olho, size);
			}
			olhos.push_back(olho);
		}
	}
	Mat result = olhos[0];
	for(int y = 0; y < result.rows; y++)
	{
		for(int x = 0; x < result.cols ;x++)
		{
			int somatorio = 0;
			for(int l = 0; l < olhos.size() ;l++)
			{
				somatorio += olhos[l].at<uchar>(Point(x,y));
//				imshow("olhos", olhos[l]);
//				waitKey(300);
			}
			//cout << "somatorio = " << somatorio << ""
			result.at<uchar>(Point(x,y)) = somatorio / olhos.size();
		}
	}
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_PXM_BINARY);
	compression_params.push_back(0);
	imwrite("images/olhos/media.pgm", result, compression_params);
	imshow("media", result);
//	waitKey(0);
//	exit(0);
}



