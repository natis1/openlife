# openlife

Simulation for studying sustained complexity of deterministic rule based systems

Main dependency is [SFML](http://www.sfml-dev.org/license.php)

![Main Sim](http://i.imgur.com/jgBq3Nx.png)

Debug mode:

![Debug Mode](http://i.imgur.com/bkwnTHQ.png)


### A density graph (Basic analysis):

![Density Graph](http://i.imgur.com/4DIB9a0.png)

### Average Location Graph:

![Location Graph](http://i.imgur.com/kAvGROO.png)

## Build Instructions

### Get prerequisite programs

To build openlife, you will need a compiler, git, make, and cmake, as well as the sfml libraries

On most Linux these should be included in your repositories. For example you might try

Ubuntu or Debian `apt-get install build-essential g++ git cmake libsfml-dev`

Arch `pacman -S gcc git cmake sfml`

Redhat or Fedora (etc.) `yum install gcc-c++ git cmake SFML`

### Build:

For most users, building should be as simple as running `./build.sh`, which will compile openlife in a build directory and copy it up one level.
When openlife is fully released it will include more features like installing.

To manually build, run the following commands:

```
mkdir build
cd build
cmake ..
```
(configure openlife with a program like ccmake)
```
make
```
optional: `make install`
