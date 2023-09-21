import matplotlib.pyplot as plt

n_thread = [1, 2, 4, 8]

omp_time = [462, 245, 138, 100]
pth_time = [591, 400, 362, 467]

plt.figure(figsize=(10, 6))
plt.axhline(y=447, color='r', linestyle='--', label='Serial')
plt.plot(n_thread, omp_time, marker='o', color='g', label='OpenMP')
plt.plot(n_thread, pth_time, marker='o', color='b', label='Pthreads')

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução (s)')
plt.title('')
plt.xticks(n_thread)
plt.legend()
plt.grid()
plt.show()

omp_time = [934, 476, 233, 221]
pth_time = [1200, 742, 600, 727]

plt.figure(figsize=(10, 6))
plt.axhline(y=941, color='r', linestyle='--', label='Serial')
plt.plot(n_thread, omp_time, marker='o', color='g', label='OpenMP')
plt.plot(n_thread, pth_time, marker='o', color='b', label='Pthreads')

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução (s)')
plt.title('')
plt.xticks(n_thread)
plt.legend()
plt.grid()
plt.show()