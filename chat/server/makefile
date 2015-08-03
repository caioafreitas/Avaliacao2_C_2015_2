include cfg.mak

all: $(DIST) $(OBJ) $(DIST)/$(EXE)
	@echo Compilado!

$(DIST)/$(EXE): $(OBJS)
	@echo Gerando $(EXE) ...
	@gcc $(OBJS) $(GCCARGS) -o $(DIST)/$(EXE)

$(OBJ)/%.o: $(SRC)/%.c
	@echo Compilando $< ...
	@gcc -c $< $(GCCARGS) -I ./$(INC) -o $@

clean:
	@rm -rf $(OBJ)/*.o $(DIST)/$(EXE)
	@echo Limpo!

remake: clean all