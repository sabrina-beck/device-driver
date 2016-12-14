# Projeto 3 - Implementação de um device driver
## Grupo 7
- Caio Cesar de Campos Silva - 160138
- Lucas Cleto de Oliveira - 156345
- Sabrina Beck Angelini - 157240

## Descrição
Implementação de um device driver que fornece frames de uma animação em asciiart. A implementação do driver pode ser visualizada no arquivo `asciiart.c`.

## Abordagem para teste
Para testar o device driver é possível utilizar o programa implementado em `ex-asciiart.c` que permite visualizar o frame atual, ir para o próximo, voltar um frame e voltar ao frame inicial.

## Apresentação
### Casos de Teste
**Caso 1**: Animação cíclica, passando do último para o primeiro frame
- No programa de teste, dar o comando `3. Next` continuamente até reiniciar a animação

**Caso 2**: Animação cíclica, passando do primeiro para o último frame
- No programa de teste, dar o comando `4. Previous` continuamente até a animação ir para o estado final

**Caso 3**: Teste do reset
- No programa de teste, dar o comando `2. Reset`
- Dar o comando `1. Current` e verificar se o frame inicial é o atual

**Caso 4**: Teste do current
- No programa de teste, dar o comando `1. Current` e verificar se o frame mostrado é o mesmo da última animação
   - Caso seja o primeiro comando executado no programa de teste, o frame inicial deve ser exibido, pois ele é o atual
