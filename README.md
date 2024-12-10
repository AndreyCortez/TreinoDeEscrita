O seguinto projeto foi feito para a matéria de sistemas operacionais.

Nele, nós temos um jogo de digitação que foi elaborado usando semaforos
e threads.

O uso de threads é feito ao calcular as estatísticas de velocidade de digitação
a cada 5 segundos a função `void* calcular_estatisticas(void* arg)`
calcula 2 métricas diferentes, a quantidade de palavras digitadas por minuto
(wpm) e a quantidade de teclas por minuto (cpm). Isso tudo acontece em uma
trhead paralela à principal.

De recursos compartilhados entre as threads temos as variaveis `word_count` e 
`char_count`. A thread principal é responsável por incrementar essas variáveis
de acordo com o input do jogador e a thread das estatísticas faz calculos com
essas variáveis e as zera logo em seguida.

Caso não houvesse algum tipo de proteção do acesso dessas variaveis, corre o risco
de algumas das adiçoes feitas pela thread principal serem perdidas durante o uso 
dessa memória compartilhada.

```c
#thread secundária
wpm = word_count / 5.0 # calcula palavras por minuto


# muda para a trhead principal
word_count += 1 # uma palavra adicional foi digitada

# muda para a thread secundária
word_count = 0 # zera as palavras, perdendo assim a palavra que foi 
	       # digitada antes

```

Para resolver isso usamos os semaforos, que impedem que as threads primária
e secundaria, acessem essa memória crítica ao mesmo tempo.
