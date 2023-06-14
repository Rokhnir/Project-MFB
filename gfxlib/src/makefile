# Chemins des fichiers objets et des fichiers d'en-tête
obj_dir := ../../temp
inc_dir := ../include

# Liste des fichiers sources
src_files := BmpLib.c ErreurLib.c ESLib.c GfxLib.c OutilsLib.c SocketLib.c ThreadLib.c VectorLib.c WavLib.c

# Liste des fichiers objets nécessaires pour la bibliothèque
obj_files := $(patsubst %.c, $(obj_dir)/%.o, $(src_files))

# Règle pour générer la bibliothèque
$(obj_dir)/libisentlib.a: $(obj_files)
	ar rcs $@ $^
	ranlib $@

# Règle générale pour la compilation des fichiers objets
$(obj_dir)/%.o: %.c
	gcc -Wall -O2 -o $@ -c $< -I$(inc_dir)

# Règle spécifique pour la compilation de GfxLib.c
$(obj_dir)/GfxLib.o: GfxLib.c $(inc_dir)/GfxLib.h $(inc_dir)/ESLib.h
	gcc -Wall -O2 -o $@ -c $< -I$(inc_dir) -I/usr/include/GL

#test: test.c libisentlib.a
	#gcc -Wall -o output test.c $(obj_dir)/libisentlib.a -lm -lglut -lGL -lX11

clean:
	rm -f $(obj_dir)/*.o $(obj_dir)/libisentlib.a
