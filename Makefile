CC = gcc
LANG = c
STD = c99
OBJ_DIR = objs/
SRC_DIR = src/
PROG_NAME = handshake
OBJS = $(OBJ_DIR)main.o \
	   $(OBJ_DIR)graphics.o \
	   $(OBJ_DIR)get_key.o \
	   $(OBJ_DIR)get_files.o \
	   $(OBJ_DIR)game_status.o \
	   $(OBJ_DIR)play_level.o \
	   $(OBJ_DIR)select_level.o \
	   $(OBJ_DIR)welcome_intro.o \
	   $(OBJ_DIR)initialize_level.o \
	   $(OBJ_DIR)initialize_textures.o \
	   $(OBJ_DIR)initialize_window.o \
	   $(OBJ_DIR)backward_move.o \
	   $(OBJ_DIR)initialize_audio.o \
	   $(OBJ_DIR)body_move.o \
	   $(OBJ_DIR)check_activators.o \
	   $(OBJ_DIR)hand_and_arm.o \
	   $(OBJ_DIR)make_move.o \
	   $(OBJ_DIR)none_move.o \
	   $(OBJ_DIR)object_move.o

CFLAGS= -Ofast -Wall -Wextra -x $(LANG) --std=$(STD)\
		$(shell pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)

LDFLAGS = -lm $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_mixer)
$(PROG_NAME) : $(OBJS)
	@$(CC) -o $(PROG_NAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)main.o :$(SRC_DIR)main.c \
		$(SRC_DIR)game_types.h \
		$(SRC_DIR)sdl_types.h \
		$(SRC_DIR)initializers.h \
		$(SRC_DIR)welcome_intro.h \
		$(SRC_DIR)matrix_legend.h \
		$(SRC_DIR)get_files.h \
		$(SRC_DIR)select_level.h \
		$(SRC_DIR)play_level.h \
		$(SRC_DIR)graphics.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)main.c -o $(OBJ_DIR)main.o

$(OBJ_DIR)graphics.o : $(SRC_DIR)graphics.c \
			$(SRC_DIR)matrix_legend.h \
			$(SRC_DIR)game_types.h \
			$(SRC_DIR)sdl_types.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)graphics.c -o $(OBJ_DIR)graphics.o

$(OBJ_DIR)get_key.o : $(SRC_DIR)get_key.c \
			$(SRC_DIR)matrix_legend.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)get_key.c -o $(OBJ_DIR)get_key.o

$(OBJ_DIR)get_files.o : $(SRC_DIR)get_files.c
	@$(CC) $(CFLAGS) -c $(SRC_DIR)get_files.c -o $(OBJ_DIR)get_files.o

$(OBJ_DIR)game_status.o : $(SRC_DIR)game_status.c \
				$(SRC_DIR)game_types.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)game_status.c -o $(OBJ_DIR)game_status.o

$(OBJ_DIR)play_level.o : $(SRC_DIR)play_level.c \
				$(SRC_DIR)matrix_legend.h \
				$(SRC_DIR)game_types.h \
				$(SRC_DIR)make_move.h \
				$(SRC_DIR)initializers.h \
				$(SRC_DIR)get_key.h \
				$(SRC_DIR)game_status.h \
				$(SRC_DIR)fps.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)play_level.c -o $(OBJ_DIR)play_level.o

$(OBJ_DIR)select_level.o : $(SRC_DIR)select_level.c \
					$(SRC_DIR)game_types.h \
					$(SRC_DIR)sdl_types.h \
					$(SRC_DIR)matrix_legend.h \
					$(SRC_DIR)graphics.h \
					$(SRC_DIR)get_key.h \
					$(SRC_DIR)fps.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)select_level.c -o $(OBJ_DIR)select_level.o

$(OBJ_DIR)welcome_intro.o : $(SRC_DIR)welcome_intro.c \
					$(SRC_DIR)matrix_legend.h \
					$(SRC_DIR)game_types.h \
					$(SRC_DIR)sdl_types.h \
					$(SRC_DIR)get_key.h \
					$(SRC_DIR)graphics.h \
					$(SRC_DIR)fps.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)welcome_intro.c -o $(OBJ_DIR)welcome_intro.o

$(OBJ_DIR)initialize_level.o : $(SRC_DIR)initializers/initialize_level.c \
						$(SRC_DIR)game_types.h \
						$(SRC_DIR)matrix_legend.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)initializers/initialize_level.c -o \
		$(OBJ_DIR)initialize_level.o

$(OBJ_DIR)initialize_textures.o : $(SRC_DIR)initializers/initialize_textures.c \
						$(SRC_DIR)sdl_types.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)initializers/initialize_textures.c -o \
		$(OBJ_DIR)initialize_textures.o

$(OBJ_DIR)initialize_window.o : $(SRC_DIR)initializers/initialize_window.c
	@$(CC) $(CFLAGS) -c $(SRC_DIR)initializers/initialize_window.c -o \
		$(OBJ_DIR)initialize_window.o

$(OBJ_DIR)initialize_audio.o : $(SRC_DIR)initializers/initialize_audio.c \
						$(SRC_DIR)sdl_types.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)initializers/initialize_audio.c -o \
		$(OBJ_DIR)initialize_audio.o

$(OBJ_DIR)backward_move.o : $(SRC_DIR)moves/backward_move.c \
					$(SRC_DIR)matrix_legend.h \
					$(SRC_DIR)game_types.h \
					$(SRC_DIR)moves/hand_and_arm.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/backward_move.c -o \
		$(OBJ_DIR)backward_move.o

$(OBJ_DIR)body_move.o : $(SRC_DIR)moves/body_move.c \
				$(SRC_DIR)matrix_legend.h \
				$(SRC_DIR)moves/none_move.h \
				$(SRC_DIR)game_types.h \
				$(SRC_DIR)moves/check_activators.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/body_move.c -o $(OBJ_DIR)body_move.o

$(OBJ_DIR)check_activators.o : $(SRC_DIR)moves/check_activators.c \
						$(SRC_DIR)game_types.h \
						$(SRC_DIR)matrix_legend.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/check_activators.c -o \
		$(OBJ_DIR)check_activators.o

$(OBJ_DIR)hand_and_arm.o : $(SRC_DIR)moves/hand_and_arm.c \
					$(SRC_DIR)matrix_legend.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/hand_and_arm.c \
		-o $(OBJ_DIR)hand_and_arm.o

$(OBJ_DIR)make_move.o : $(SRC_DIR)moves/make_move.c \
				$(SRC_DIR)moves/none_move.h\
				$(SRC_DIR)moves/backward_move.h \
				$(SRC_DIR)moves/object_move.h \
				$(SRC_DIR)moves/body_move.h \
				$(SRC_DIR)moves/check_activators.h \
				$(SRC_DIR)game_types.h \
				$(SRC_DIR)matrix_legend.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/make_move.c -o $(OBJ_DIR)make_move.o

$(OBJ_DIR)none_move.o : $(SRC_DIR)moves/none_move.c \
				$(SRC_DIR)moves/hand_and_arm.h \
				$(SRC_DIR)game_types.h \
				$(SRC_DIR)matrix_legend.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/none_move.c -o $(OBJ_DIR)none_move.o

$(OBJ_DIR)object_move.o : $(SRC_DIR)moves/object_move.c \
							$(SRC_DIR)matrix_legend.h \
							$(SRC_DIR)game_types.h \
							$(SRC_DIR)moves/none_move.h
	@$(CC) $(CFLAGS) -c $(SRC_DIR)moves/object_move.c -o \
		$(OBJ_DIR)object_move.o

run:
	./$(PROG_NAME) levels/

.PHONY : clean
clean :
	rm $(PROG_NAME) $(OBJS)
