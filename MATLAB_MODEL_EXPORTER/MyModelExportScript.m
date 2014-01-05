load face_p146_small.mat
nFilters=size(model.filters,2);

for i=1:nFilters
    fname=strcat('.\Filters\w',int2str(i),'.txt');
    saveMatrix(fname,model.filters(1,i).w); % w
    fname=strcat('.\Filters\i',int2str(i),'.txt');
    dlmwrite(fname,model.filters(1,i).i); % w
end;

nDefs=size(model.defs,2);
for i=1:nDefs
    fname=strcat('.\Defs\w',int2str(i),'.txt');
    saveMatrix(fname,model.defs(1,i).w); % w
    fname=strcat('.\Defs\i',int2str(i),'.txt');
    dlmwrite(fname,model.defs(1,i).i); % i
    fname=strcat('.\Defs\anchor',int2str(i),'.txt');
    saveMatrix(fname,model.defs(1,i).anchor); % anchor
end;

nComp=size(model.components,2);
for i=1:nComp
    filename=strcat('.\Components\component',int2str(i),'.txt');
    nCells=size(model.components{1,i},2);
    dlmwrite(filename, [size(model.components{1,i},2),3]);
    for j=1:nCells
        defid=model.components{1,i}(1,j).defid;
        filterid=model.components{1,i}(1,j).filterid;
        parent= model.components{1,i}(1,j).parent ;
        M=[ defid,filterid, parent ]
        dlmwrite (filename,M,'-append');
    end;
end;

filename='.\MaxSize\maxsize.txt';
saveMatrix(filename,model.maxsize);

filename='.\Len\len.txt';
dlmwrite(filename,model.len);

filename='.\Interval\interval.txt';
dlmwrite(filename,model.interval);

filename='.\Sbin\sbin.txt';
dlmwrite(filename,model.sbin);

filename='.\Delta\delta.txt';
dlmwrite(filename,model.delta);

filename='.\Thresh\thresh.txt';
dlmwrite(filename,model.thresh);

filename='.\Obj\obj.txt';
dlmwrite(filename,model.obj);
