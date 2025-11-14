import pandas as pd
import os
import matplotlib.pyplot as plt

# Caminho do arquivo Excel
arquivo_excel = 'Planilha sem título.xlsx'

# Lê a planilha Excel
df = pd.read_excel(arquivo_excel)

# Converte para CSV com separador ;
df.to_csv('produtosTechParts.csv', sep=';', index=False, encoding='utf-8')
