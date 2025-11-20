import os
import gspread
from gspread_formatting import *
import json
from google.oauth2.service_account import Credentials

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

def parse_white_cells_to_json(sheet, output_file='graph.json'):
    """
    Парсит белые клетки как вершины графа
    Координаты вершины = (row, column) ячейки
    Белый цвет: RGB(1, 1, 1)
    """
    graph = {}
    
    try:
        # Получаем все ячейки с форматированием
        all_data = sheet.get_all_values()
        num_rows = len(all_data)
        num_cols = len(all_data[0]) if all_data else 0
        print(f"Таблица имеет размеры: {num_rows}x{num_cols}")

        if num_rows == 0 or num_cols == 0:
            print("Таблица пуста.")
            return {}

        white_cells = []
        _ = 0
        # Проходим по всем ячейкам и проверяем их форматирование
        for row in range(1, num_rows + 1):
            for col in range(1, num_cols + 1):
                try:
                    # Получаем формат конкретной ячейки
                    cell_format = get_user_entered_format(sheet, f'{gspread.utils.rowcol_to_a1(row, col)}')

                    if True:
                        color = cell_format.backgroundColor

                        if color.green == 1 and color.blue == 1 and color.red == 1 :
                            print("Белая клетка", col, row, _)
                            _ += 1
                            white_cells.append((col - 1, row - 1))

                            
                except Exception as cell_error:
                    # Пропускаем ячейки с ошибками форматирования
                    continue
        
        # Теперь для каждой белой ячейки ищем соседей
        for row, col in white_cells:
            neighbors = []
            vertex = (row, col)
            
            # Проверяем 4-связность (верх, низ, лево, право)
            directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]  # up, down, left, right
            
            for dr, dc in directions:
                neighbor_row, neighbor_col = row + dr, col + dc
                neighbor_coord = (neighbor_row, neighbor_col)
                
                if neighbor_coord in white_cells:
                    neighbors.append(neighbor_coord)
            
            # Добавляем вершину в граф
            graph[str(vertex)] = [str(n) for n in neighbors]
            print(f"✅ Вершина {vertex} -> соседи: {neighbors}")
    
    except Exception as e:
        print(f"❌ Ошибка при парсинге: {e}")
        return
    
    # Сохраняем в JSON
    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            json.dump(graph, f, indent=4, ensure_ascii=False)
        print(f"✅ Граф сохранен в {output_file}")
        print(f"📊 Всего вершин: {len(graph)}")
    except Exception as e:
        print(f"❌ Ошибка при сохранении JSON: {e}")

# Запускаем парсинг
if __name__ == "__main__":
    parse_white_cells_to_json(sheet)