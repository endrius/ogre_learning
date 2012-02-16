/******************************************************************************
*
* COPYRIGHT Jonathan Ohara de Araujo ALL RIGHTS RESERVED.
*
* This software cannot be copied, stored, distributed without
* Vinícius G.Mendonça prior authorization.
*
* This file was made available on http://www.pontov.com.br and it is free
* to be restributed or used under Creative Commons license 2.5 br:
* http://creativecommons.org/licenses/by-sa/2.5/br/
*
*******************************************************************************
* Este software nao pode ser copiado, armazenado, distribuido sem autorização
* a priori de Jonathan Ohara de Araujo
*
* Este arquivo foi disponibilizado no site http://www.pontov.com.br e esta
* livre para distruição seguindo a licença Creative Commons 2.5 br:
* http://creativecommons.org/licenses/by-sa/2.5/br/
*
******************************************************************************/

#include <ctime>
#include <cstdlib>
#include <iostream>

#include <OGRE/Ogre.h>

using namespace std;

#ifdef _DEBUG
#define PLUGINS_CFG "plugins_d.cfg"
#else
#define PLUGINS_CFG "plugins.cfg"
#endif

class Tutorial1 {
private:
	Ogre::Root *mRoot;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;

public:
	Tutorial1(void){}
	~Tutorial1(void){
		delete mRoot;
	}

	bool initializeOgreStuff(){
		srand( (unsigned)time( NULL ) );

		// Construi o Root do Ogre
		mRoot = new Ogre::Root(PLUGINS_CFG);

		// Carregando as pastas dos resources
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/materials/scripts", "FileSystem", "Popular");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/materials/textures", "FileSystem", "Popular");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media/models", "FileSystem", "Popular");
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("media", "FileSystem", "General");

		// mRoot->restoreConfig() Verifica se ja existe configuração no ogre.cfg
		// se existir retorna true e entra no if
		// senão cai no OU e mostra a tela para escolher configuração
		if(mRoot->restoreConfig() || mRoot->showConfigDialog()){
			// O primeiro parâmetro diz ao Ogre que ele pode criar uma janela para renderização
			// O segundo parâmetro é o titulo da Janela
			mWindow = mRoot->initialise(true, "Ponto V - Tutorial 1");
		}else{
			// Caso o usuário aperte cancel na tela do ogre feche o programa
			return false;
		}

		// Escolhendo o gerenciador de cenas
		mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

		// Cria a camera
		mCamera = mSceneMgr->createCamera("Camera");

		// ViewPort é o que faz a cena ser renderizado pela camera
		Ogre::Viewport* vp = mWindow->addViewport(mCamera);
		// Cor de fundo do ViewPort
		vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

		// Muda o aspecto(width, height) da camera para se adequar ao ViewPort
		mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
		// Coloca o número padrão de MipMaps
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

		// Carrega Todos resoures
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		return true;
	}

	bool runApplication(){
		// Cria um array com meshes que vem no sdk do ogre
		Ogre::String enityNames[] = {"fish", "knot", "ninja", "ogrehead", "penguin", "razor", "robot", "WoodPallet"};

		// Posiciona a Camera
		mCamera->setPosition(Ogre::Vector3(0, 0, 200));
		// Manda a camera olhar para um ponto
		mCamera->lookAt(Ogre::Vector3(0, 0, -300));
		// Distancia mínima para que o objeto deve estar da camera para ser renderziado
		mCamera->setNearClipDistance(5);
		// Distancia máxima para que o objeto deve estar da camera para ser renderziado
		mCamera->setFarClipDistance(500);

		//Gero um número randomico entre 0 e 7
		int randomMeshPosition = rand() % 8;

		// Crio um nó filho do nó raiz na hierarquia de cena
		Ogre::SceneNode* node1 = mSceneMgr->getRootSceneNode()->createChildSceneNode("no1");
		// Crio uma entidade nesse caso um mesh a partir do resultado randomico
		Ogre::Entity* entity = mSceneMgr->createEntity( enityNames[randomMeshPosition], enityNames[randomMeshPosition].append(".mesh"));
		// Coloco esse mesh no nó
		node1->attachObject(entity);

		// Cria Luz ambiente
		mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

		// Cria uma Luz
		Ogre::Light* l = mSceneMgr->createLight("Luz");
		l->setPosition(20, 80, 50);

		//Loop Principal
		for(;;){
			// Processa as mensagens que o Sistema Operaciona envia para aplicação
			Ogre::WindowEventUtilities::messagePump();

			// Parar o programa caso a janela seja fechada
			if(mWindow->isClosed()){
				return false;
			}

			node1->yaw(Ogre::Degree(0.01));

			// Renderiza Um Frame
			if(!mRoot->renderOneFrame()) return false;
		}
		return false;
	}
};

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
		Tutorial1 application;
        try {
			if( application.initializeOgreStuff() ){
				application.runApplication();
			}
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
