import os
import gspread
import json
from google.oauth2.service_account import Credentials
from gspread_formatting import *
import time

def read_path_from_file(filename="path.txt"):
    with open(filename, 'r') as file:
        content = file.read().strip()
    
    # Разбиваем по пробелам и создаем кортежи
    path = []
    for vertex in content.split(') '):
        # Убираем скобки и разделяем по запятой
        clean = vertex.strip('()')
        x, y = map(int, clean.split(','))
        path.append((x, y))
    
    return path



# 📊 Подключаемся к Google Таблице
try:
    scope = ['https://spreadsheets.google.com/feeds', 'https://www.googleapis.com/auth/drive']
    creds = Credentials.from_service_account_file('credentials.json', scopes=scope)
    client = gspread.authorize(creds)
    sheet = client.open("maze").sheet1
    print("✅ Google Sheets подключен")
except Exception as e:
    print(f"❌ Ошибка Google Sheets: {e}")
    exit()

def color_path_cells(sheet, path, color=(1, 1, 0)):  # Желтый цвет по умолчанию
    """
    Красит ячейки из пути в указанный цвет
    
    Args:
        sheet: объект листа Google Sheets
        path: список кортежей [(row, col), ...]
        color: кортеж RGB (red, green, blue) от 0 до 1
    """
    try:
        # Создаем форматирование
        fmt = CellFormat(
            backgroundColor=Color(*color)
        )
        
        # Красим каждую ячейку из пути
        for col, row in path:
            # Преобразуем координаты в формат A1 (например, (1,1) -> 'A1')
            cell_label = gspread.utils.rowcol_to_a1(row+1, col+1)
            
            # Применяем форматирование
            format_cell_range(sheet, cell_label, fmt)
            
            print(f"✅ Покрашена ячейка {cell_label} ({row}, {col})")
            
            # Небольшая задержка чтобы не превысить лимиты API
            time.sleep(0.1)
        
        print(f"✅ Все ячейки пути покрашены в желтый цвет")
        
    except Exception as e:
        print(f"❌ Ошибка при покраске ячеек: {e}")

# Твой путь (замени на реальные данные)
path = read_path_from_file()

# Красим ячейки
color_path_cells(sheet, path)



print(path) 