import os

PATH = r'''Z:\1'''

def RemoveCRLF(dirname):
    os.chdir(dirname)
    files = os.listdir()
    for fn in files:
        if fn[0] == '0':
            continue
        if os.path.splitext(fn)[1] == '.txt':
            print('proc file %s' % fn)
            gn = 't' + fn;
            f = open(fn, 'r')
            g = open(gn, 'w')
            for s in f:
                g.write(s.strip() + ' ')
            f.close()
            g.close()
            os.remove(fn)
            os.rename(gn, fn)
    os.chdir('..')

if __name__ == '__main__':
    os.chdir(PATH)
    files = os.listdir()
    for fn in files:
        if os.path.isdir(fn):
            print('****proc dir %s' % fn)
            RemoveCRLF(fn)