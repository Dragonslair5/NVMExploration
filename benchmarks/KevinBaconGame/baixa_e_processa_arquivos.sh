#!/bin/bash

wget https://github.com/dedeler/imdb-data-parser/archive/master.zip
unzip -o master.zip
rm master.zip
cd imdb-data-parser-master/

CONF_FILE="idp/settings.py"
echo "INPUT_DIR = \"input/\"" > $CONF_FILE
echo "OUTPUT_DIR = \"output/\"" >> $CONF_FILE
echo "INTERFACES_SERVER = \"ftp.fu-berlin.de\"" >> $CONF_FILE
echo "INTERFACES_DIRECTORY = \"pub/misc/movies/database/\"" >> $CONF_FILE
echo "LISTS = [\"actors\", \"actresses\"]" >> $CONF_FILE

if [[ -f input/actors.list && -f input/actresses.list ]]; then
    ./imdbparser.py
else
    mkdir input 2> /dev/null
    ./imdbparser.py -u
fi

cd ..
FNAME=atores
rm $FNAME 2>/dev/null

echo "Iniciando processamento. Isto pode levar um bom tempo..."
for FINOME in `find imdb-data-parser-master -name *.list.tsv`
do
    echo "Processando $FINOME"
    cat "$FINOME" |
    cut -f 1,2,3 | #seleciona colunas nome, sobrenome e filme
    sed 's/\t/ / ' | #junta nome e sobrenome tirando o primeiro tab
    sed 's/{.*}//' | #remove informacoes sobre episodios
    grep -Pv '^.+\t[^\t]+\(TV\)' | #remove programas de TV 
    sed 's/(V)//' |
    sed 's/ *$//' | #trim right
    sed -r 's/\(([^)]+)\)$/\t\1/' | #remove parenteses do final (ano) e coloca tab
    sed -r 's/ \t/\t/g' | #remove eventuais espacos + tab para um unico tab
    grep -Pv '^.+\t".+"\t.+$' | #Segundo as regras, titulos entre aspas sao series, queremos apenas filmes
    sed -r 's/(.+\t.+\t[0-9]+)\/.*$/\1/' | #Limpa o ano da producao
    sort -u >> $FNAME
done

#Se houver mais de um dump no diretorio podem haver duplicatas, aqui assegura-se que isso não vai ocorrer
cat $FNAME | sort -u > $FNAME.tmp
mv $FNAME.tmp $FNAME

echo "Gerando atores a partir de 2010"
awk -F"\t" '$3 >= "2010" && $3 <= "2030" { print $1"\t"$2}' $FNAME > $FNAME.2010 

echo "Gerando atores a partir de 2015"
awk -F"\t" '$3 >= "2015" && $3 <= "2030" { print $1"\t"$2}' $FNAME > $FNAME.2015 

echo "Limpando arquivo de atores"
cut -f 1,2 $FNAME > $FNAME.completo
rm $FNAME

echo ================================
echo ================================
echo Você já pode apagar o diretório imdb-data-parser-master para liberar espaço em disco.
echo Os arquivos $FNAME e $FNAME.2010 foram gerados com sucesso

