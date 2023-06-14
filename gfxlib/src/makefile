# Chemins des fichiers objets et des fichiers d'en-tête
obj_dir := ../../temp
inc_dir := ../include

# Liste des fichiers sources
src_files := BmpLib.c ErreurLib.c ESLib.c GfxLib.c OutilsLib.c SocketLib.c ThreadLib.c VectorLib.c WavLib.c

# Liste des fichiers objets nécessaires pour la bibliothèque
obj_files := $(patsubst %.c, $(obj_dir)/%.o, $(src_files))

# Règle pour générer la bibliothèque
libisentlib.a: $(obj_files)
	ar rcs $@ $^
	ranlib $@

# Règle générale pour la compilation des fichiers objets
$(obj_dir)/%.o: %.c
	gcc -Wall -O2 -o $@ -c $< -I$(inc_dir)

test: test.c libisentlib.a
	gcc -Wall -o output test.c libisentlib.a -lm -lglut -lGL -lX11

clean:
	rm -f $(obj_dir)/*.o libisentlib.a output