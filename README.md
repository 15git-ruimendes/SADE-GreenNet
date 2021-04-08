# SADE-GreenNet

## OBJETIVOS

    Minimizar o custo (DISTANCIA * Kc)
    Maximizar a proximidade
    Minimizar o tempo (DISTANCIA * Kt)
    Maximizar a quantidade (Soma de necessidadades)


## RESTRIÇÕES

    A soma das necessidadades de cada loja afetada por cada armazem tem que ser menor que a capacidade;
    A necessidade de cada loja tem que ser totalmente abastecida;
    Cada loja só pode ser servida por um armazem;

## HEURISTICAS CONSTRUTIVAS
### Clusters
    Fazer o quociente Ni/Dij (max).
    Selecionar os maximos até atingir a capacidade.
### Rotas
    Minimizar distancia entre todos os pontos do Clusters.
    Selecionar o ponto com menor distancia ao armazem.
    Selecionar o ponto com menor distancia á loja atual (retirando a loja atual da lista de pontos a passar)
    Fazer isto até esgotar a lista de pontos. 

## ALGORITMOS

### Problema de Clusters
    Anealing
### Problema de Rotas
    Genetic

## RELATORIO
https://www.overleaf.com/project/606c68875dd303926aeb7dd6
