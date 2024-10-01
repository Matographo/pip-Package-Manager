#include "pip.h"

Pip::Pip() {
}

Pip::~Pip() {
}

int Pip::install(std::string package) {
    std::string pipCall = "pip install " + package;
    return system(pipCall.c_str());
}

int Pip::install(std::vector<std::string> packages) {
    std::string pipPackages = "";
    for(std::string package : packages) {
        pipPackages += package + " ";
    }
    return this->install(pipPackages);
}

int Pip::uninstall(std::string package) {
    std::string pipCall = "pip uninstall " + package;
    return system(pipCall.c_str());
}

int Pip::uninstall(std::vector<std::string> packages) {
    std::string pipPackages = "";
    for(std::string package : packages) {
        pipPackages += package + " ";
    }
    return this->uninstall(pipPackages);
}

int Pip::update(std::string package) {
    std::string pipCall = "pip install --upgrade " + package;
    return system(pipCall.c_str());
}

int Pip::update(std::vector<std::string> packages) {
    std::string pipPackages = "";
    for(std::string package : packages) {
        pipPackages += package + " ";
    }
    return this->update(pipPackages);
}

int Pip::update() {
    std::string pipCall = "pip list --outdated --format=freeze | grep -v '^\\-e' | cut -d = -f 1 | xargs -n1 pip install -U";
    return system(pipCall.c_str());
}

int Pip::search(std::string package) {
    std::string pipCall = "pip search " + package;
    return system(pipCall.c_str());
}

int Pip::list() {
    std::string pipCall = "pip list";
    return system(pipCall.c_str());
}

int Pip::info(std::string package) {
    std::string pipCall = "pip show " + package;
    return system(pipCall.c_str());
}

void Pip::setToolkit(PackageManagerToolkit * toolkit) {
    this->toolkit = toolkit;
    this->checkPip();
}

void Pip::checkPip() {
    std::string pipCall = "pip --version > /dev/null 2>&1";
    
    if(system(pipCall.c_str()) == 0) {
        return;
    }
    
    std::string pythonCall = "python --version > /dev/null 2>&1";

    if(system(pythonCall.c_str()) != 0) {
        this->toolkit->installInSys("python");
    }
    
    #ifdef _WIN32
        // Auf Windows verwenden wir das python -m pip install Befehlsformat
        system("python -m ensurepip");
        system("python -m pip install --upgrade pip");
    #else
        // Auf Linux oder MacOS verwenden wir get-pip.py oder apt-get
        system("curl https://bootstrap.pypa.io/get-pip.py -o get-pip.py");
        system("python get-pip.py");
        system("rm get-pip.py");  // Lösche das Skript nach der Installation
    #endif
}


extern "C" PackageManager * create() {
    return new Pip();
}

extern "C" void destroy(PackageManager * pm) {
    delete pm;
}