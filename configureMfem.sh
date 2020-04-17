 #! /bin/#!/usr/bin/env bash
 cd ..;
 #if mfem directory exists then stash and update it,
 #else download the lastest version.
 if [[ -d "mfem" ]]; then
   echo "Mfem directory found!"
   cd mfem;
   echo "Stashing current changes and pulling latest from mfem github repo."
   git stash;
   git pull https://github.com/mfem/mfem.git;
   cd ..
 else
   echo "Mfem directory Not found!";
   echo "Downloading latest mfem repo from github!";
   git clone https://github.com/mfem/mfem.git;
 fi

 #Copying custom modifications to the mfem directory.
cd mfem-apps
echo "Linking custom settings to mfem directory!"
rm ../mfem/config/user.cmake;
ln -s $(pwd)/config/user.cmake ../mfem/config/user.cmake;
echo "Replacing 'defaults.cmake' with custom file. Old file renamed to 'defaults.cmake.old'!"
if [[ ! -f ../mfem/config/defaults.cmake.old ]]; then
  mv ../mfem/config/defaults.cmake ../mfem/config/defaults.cmake.old;
fi

rm ../mfem/config/defaults.cmake;
ln -s $(pwd)/config/defaults.cmake ../mfem/config/defaults.cmake;
echo "Replacing 'config/cmake/modules' with custom file. Old file renamed to 'config/cmake/modules.old'!"
if [[ ! -d ../mfem/config/cmake/modules.old ]]; then
  mv ../mfem/config/cmake/modules ../mfem/config/cmake/modules.old;
fi
rm ../mfem/config/cmake/modules;
ln -s $(pwd)/config/cmake/modules ../mfem/config/cmake/modules;

echo "Creating link to external mfem directory inside source directory!"
rm mfem;
ln -s ../mfem mfem;

if [[ -d "/etc/apt" ]]; then
  echo "It seems you have an apt based package manager."
  echo "Installing required packages
  Please enter the password!"
  sudo apt-get install libopenmpi-dev libpetsc-real-dev libhypre-dev libmetis-dev libparmetis-dev libsuitesparse-dev libsuperlu-dist-dev;
else
  echo "Since you do not have an apt based package manager,
  Please install the development packages of these libraries!"
  echo "openmpi
  petsc-real
  hypre
  metis
  parmetis
  suitesparse
  libsuperlu-dist"
fi
#Build mfem as a library

#cd ..
#if [[ -d "mfem-build" ]]; then
#  echo "Pre-Existing 'mfem-build' directory Found!"
#else
#  echo "Making a build directory named mfem-build"
#  mkdir mfem-build;
#fi
#cd mfem-build;
#cmake ../mfem/;
#make -j4;
