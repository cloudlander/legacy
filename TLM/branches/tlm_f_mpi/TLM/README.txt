                                    TLMʹ��˵��

0.  ϵͳ����:
    Windows XP(��) ���ϲ���ϵͳ  (�谲װPython 2.4)
    ����
    Linux (��Python2.4,  gnuplot,  mencoder,  mplayer)
 
1.  �ļ�Ŀ¼�ṹ:
    ��ѹ�����Ŀ¼�к���:
    \                                                                                
        TLM\                                                    TLM������Ŀ¼                                       
                Release\TLM.exe                                      TLM��ִ�г���
                EY\                                                          ���EY�������ļ�
                EY_3D\                                                     ����EY���ݻ��Ƶ�3Dͼ��
                EY_MAP\                                                  ����EY���ݻ��Ƶ�2Dͼ��
                FEY\                                                        ���FEY�������ļ�
                TLM.f90                                                    TLM��Fortran90Դ����
                twotlme.in                                                TLM�������ļ�
                gen_ani.bat                                               ����3D��2Dͼ�����ɶ����ĳ���(��Ҫ�޸�,����Windows)
                gen_ani.sh                                                 ����3D��2Dͼ�����ɶ����ĳ���(��Ҫ�޸�,����Linux)
                README.txt                                              ��˵���ļ�
                TLM.py                                                      TLM��������ִ�г���
                TLM.dsp                                                    Compac Fortran 6.5����Ŀ�ļ�(��Ҫֱ�Ӵ�)
                TLM.vfproj                                                 Visual Studio 2005����Ŀ�ļ�(��Ҫ��װIntel Fortran Compiler,��Ҫֱ�Ӵ�)
                mencoder\                                                 mencoder����(��Ҫɾ���͸Ķ�)
                gnuplot.exe                                                gnuplot����(��Ҫɾ���͸Ķ�)
                wgnuplot.exe                                             gnuplot����(��Ҫɾ���͸Ķ�)
        TLM_F.dsw                                           Compac Fortran 6.5����Ŀ�ļ�(����ֱ�Ӵ�)
        TLM_F.opt                                            ��Ŀ�ļ�(��Ҫֱ�Ӵ�)
        TLM_F.sln                                             Visual Studio 2005����Ŀ�ļ�(����ֱ�Ӵ�)
        TLM_F.suo                                            ��Ŀ�ļ�(��Ҫֱ�Ӵ�)


2. ʹ�ò���:
    0.  ��β鿴X-Z�ռ���ʷֲ�
         ��Compaq Fortran��dsw,��Run����TLM.exe���ȴ�����10֮��,��CTRL-C��ֹTLM
         ˫��wgnuplot.exe,������������:
            load 'set.gnu'
            set pm3d map
            set cbrange [-1:2]
            set zrange [-1:2]
            splot 'grid.dat'
    1.  �޸�Դ����:
         �����Ҫ�༭Դ����,��˫��TLM_F.dsw(����Ѱ�װCompac Fortran6.5)����TLM_F.sln(����Ѱ�װVisual Studio 2005�Լ�Intel Fortran Compiler)
         �༭��TLM.f90Դ�����,����ʵ����Ҫ�����Release�汾����Debug�汾
    2.  ����ʵ����Ҫ�޸�twotlme.in    (������޸Ĳ����ο���������)
    3.  ���з�ʽ:
         ��������TLM.py�����������TLM����ͻ�ͼ�Ĺ����Ѿ���ͼ����,��������ϸ��˵��:
         �����г�����TLM.py�п������õĳ����(������Notepad��TLM.py�����޸�
          self._config['threads']=2                                      ����ÿ�ֻ�ͼ���߳���Ŀ(����߳���Ϊ2,�����ͬʱҪ�õ���ά�Ͷ�ά��ͼ��,ϵͳ�ܹ�������4���̻߳�ͼ.һ����˵,ϵͳ�ܹ��������߳���Ŀ���ܴ��ڻ�����CPU��������,�������ܷ������½�)
          self._config['PNG_SIZE']='1024,768'                     ��ͼ����ļ��ķֱ���
          self._config['+RANGE']=0.3                                 EY������
          self._config['-RANGE']=-0.3                                 EY������
          self._config['+G_RANGE']=1.0                             Gauss�ֲ�������(����TLM Grid������Ч)
          self._config['-G_RANGE']=-1.0                             Gauss�ֲ�������(����TLM Grid������Ч)
          self._config['TLM']=True                                     �Ƿ�����TLM�������������ļ�(���Ѿ��������ļ��������,�������ó�False,���������ظ�����,Ҳ���Ի�ͼ)
          self._config['MAP']=True                                    �Ƿ���ƶ�άͼ��
          self._config['SURFACE']=True                             �Ƿ������άͼ��
          self._config['GAUSS']=False                                �Ƿ����Gauss�ֲ�ͼ��(����TLM Grid������Ч,������ó�True,TLM NRI����ᱨ��!)
          self._config['ANI']=True                                      �Ƿ����ɶ�ά����ά�Ķ���
 
     4.  Ƶ����㹤��:
          �ڵõ����е�������ݺ�,����˫��tlm_dft.py,������Ҫ�����X,Z����,���ɵõ���Ӧ��Ƶ�����

     5.  �������õĹ���:
          ���������Ҫ������TLM����һ��ʱ��֮��������ݵ�Ч��,�Ϳ���˫��gen_ani.bat����multi_gen_ani.bat�õ���Ӧ�Ķ�ά����ά����,gen_ani.bat�����ڽ����Ļ���,multi_gen_ani.bat���е����ɶ���,�����ʺ��ڽϿ�Ķ�˻���
          clean.bat��������������е���������ļ���ͼ���ļ�

3.  twotlme.in�����ļ��޸�˵��:
     ����