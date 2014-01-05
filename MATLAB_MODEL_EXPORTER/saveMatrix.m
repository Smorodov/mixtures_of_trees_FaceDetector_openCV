function saveMatrix(filename,M)
dlmwrite(filename, size(M));
dlmwrite (filename,M,'-append');