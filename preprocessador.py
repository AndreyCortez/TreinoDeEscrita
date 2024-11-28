# Nome do arquivo de entrada e saída
input_file = "br-sem-acentos.txt"  # Arquivo contendo as palavras separadas por newline
output_file = "lista_palavras.h"  # Arquivo .h para salvar a lista C

# Função principal
def generate_c_header(input_file, output_file):
    try:
        # Lê o conteúdo do arquivo de entrada
        with open(input_file, "r") as file:
            words = file.read().splitlines()
        
        # Remove linhas vazias
        words = [word for word in words if word.strip()]
        
        # Gera o formato da lista em C
        c_array = f"const char* words[] = {{\n"
        c_array += ",\n".join(f'    "{word}"' for word in words)
        c_array += "\n};\n"
        
        # Salva no arquivo de saída
        with open(output_file, "w") as file:
            file.write("// Generated C header file\n")
            file.write("#ifndef WORDS_H\n#define WORDS_H\n\n")
            file.write(c_array)
            file.write("\n#endif // WORDS_H\n")
        
        print(f"Arquivo .h gerado com sucesso em: {output_file}")
    except Exception as e:
        print(f"Ocorreu um erro: {e}")

# Executa a função
generate_c_header(input_file, output_file)
