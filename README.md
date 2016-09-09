# openlife

The ultimate life simulator

### Install:

Standard (Assumes `git`, `g++`, `make`)
```bash
sudo apt-get install libsfml-dev
git clone https://github.com/natis1/openlife
cd openlife
make
./build/life
```
Server (All pre-requisites)
```bash
ssh -Y [server ip]

sudo apt-get install g++
sudo apt-get install make
sudo apt-get install libsfml-dev
sudo apt-get install git
git clone https://github.com/natis1/openlife
cd openlife
make
./build/life
```

(Graphics will be output on client machine)
