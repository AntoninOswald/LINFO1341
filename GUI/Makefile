# Localisation du SDK Gmsh
#GMSH_DIR := ../gmsh-sdk
GMSH_DIR := /usr

#Choix du compilateur
CC := gcc

#Spécification du runtime path
LDFLAGS := -Wl,-rpath,$(GMSH_DIR)/lib

#Spécification des flags pour le compilateur
CFLAGS := -I$(GMSH_DIR)/include -c 

#Spécification des flags pour le linker
LDFLAGS := -L$(GMSH_DIR)/lib -lgmsh -lm -Wl,-rpath,$(GMSH_DIR)/lib

init:
	$(CC) -c src/*.c
	$(CC) -o project project.o matrix.o elasticity.o design.o $(GMSH_DIR)/lib/libgmsh.so -llapacke -llapack -lblas -lm

clean:
	rm -f project project.o matrix.o elasticity.o eigen.o design.o

test:
	./project 2 lapackEigenFreq.txt 0.0060776  0.01255774 0.04094558 0.06771359 0.3

	