<h1></h1>
<h2></h2>
<h3>1. Время работы после написания основной части:</h3><b>
        <h4>вычисление на cpu, 128*128</h4><b>
        <img src = 'Screenshot from 2023-03-08 21-03-03.png'></img><b>
        <h4>вычисление на gpu, 128*128</h4><b>
        <img src = ''></img><b>
        <p>При попытке посчитать 1024*1024 было понятно, что это займёт слишком много времени</p>
<h3>2. Изменение основного цикла программы</h3>
<p>Для изменения улучшения работы моей программы я добавил следующий код:</p>
<code>#pragma acc enter data copyin(mas [0:n] [0:m], error_c) create(local_arr[n][m])</code><br>
<code>#pragma acc parallel loop present(mas[0:n][0:m])</code><br>
<code> #pragma acc update device(error_c)</code><br>
<code>#pragma acc parallel loop collapse(2) present(mas[0:n][0:m]) reduction(max : error_c)</code><br>