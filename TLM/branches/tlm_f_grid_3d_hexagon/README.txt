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
                wgnuplot.exe                                              gnuplot����(��Ҫɾ���͸Ķ�)
                ����.dll,.exe��.bat�ļ�                                   �����������Ҫʹ��(��Ҫɾ���͸Ķ�)
        TLM_F.dsw                                           Compac Fortran 6.5����Ŀ�ļ�(����ֱ�Ӵ�)
        TLM_F.opt                                            ��Ŀ�ļ�(��Ҫֱ�Ӵ�)
        TLM_F.sln                                             Visual Studio 2005����Ŀ�ļ�(����ֱ�Ӵ�)
        TLM_F.suo                                            ��Ŀ�ļ�(��Ҫֱ�Ӵ�)


2. ʹ�ò���:
    0.  ��β鿴X-Z�ռ���ʷֲ�: 
         ��Compaq Fortran��dsw,��Run����TLM.exe���ȴ�����10֮��,��CTRL-C��ֹTLM
         ��ACDSee����Iranview�����ɵ�grid.ppmͼ���ļ�����
        
    1.  �޸�Դ����:
         �����Ҫ�༭Դ����,��˫��TLM_F.dsw(����Ѱ�װCompac Fortran6.5)����TLM_F.sln(����Ѱ�װVisual Studio 2005�Լ�Intel Fortran Compiler)
         �༭��TLM.f90Դ�����,����ʵ����Ҫ�����Release�汾����Debug�汾
         ����Release�汾�ʺ���������ʱʹ��,�ٶȽϿ�.Debug�汾���ʺϵ���ʱ��Visual Fortran������ʹ��

    2.  ����ʵ����Ҫ�޸�twotlme.in    (������޸Ĳ����ο���������)

    3.  ���з�ʽ:
         ��������TLM.py�����������TLM����ͻ�ͼ�Ĺ����Ѿ���ͼ����,��������ϸ��˵��:
         �����г�����TLM.py�п������õĳ����(������Notepad��TLM.py�����޸�
          self._config['threads']=2                   ����ÿ�ֻ�ͼ���߳���Ŀ
                                                      (����߳���Ϊ2,�����ͬʱҪ�õ���ά�Ͷ�ά��ͼ��,
                                                       ϵͳ�ܹ�������4���̻߳�ͼ.һ����˵,ϵͳ�ܹ�����
                                                       ���߳���Ŀ���ܴ��ڻ�����CPU��������,�������ܷ���
                                                       ���½�.��һ���˫�˼������˵,���ó�2����)
          self._config['PNG_SIZE']='1024,768'         ��ͼ����ļ��ķֱ���
          self._config['+RANGE']=0.3                  EY������
          self._config['-RANGE']=-0.3                 EY������
          self._config['+G_RANGE']=1.0                Gauss�ֲ�������(����TLM Grid������Ч)
          self._config['-G_RANGE']=-1.0               Gauss�ֲ�������(����TLM Grid������Ч)
          self._config['+H_RANGE']=3.0                ģ������(����TLM Grid������Ч)
          self._config['-H_RANGE']=0                  ģ������(����TLM Grid������Ч)
          self._config['PALETTE']                     ���û�ͼ�ĵ�ɫ��,PALETTE�Ǹ�EX,EY,EZ/HX,HY,HZ�õ�,G_PALETTE�Ǹ���˹�õ�
                                                      H_PALETTE�Ǹ�ģ�õ�
          self._config['TLM']=True                    �Ƿ�����TLM�������������ļ�
                                                      (���Ѿ��������ļ��������,�������ó�False,
                                                       ���������ظ�����,Ҳ���Ի�ͼ)
          self._config['MAP']=True                    �Ƿ���ƶ�άͼ��
          self._config['SURFACE']=True                �Ƿ������άͼ��
          self._config['GAUSS']=False                 �Ƿ����Gauss�ֲ�ͼ��
                                                      (����TLM Grid������Ч,����,������ó�True,TLM NRI����ᱨ��!)
          self._config['ONLY_SIN']=False              �Ƿ����SIN�����ֲ�ͼ��
                                                      (����TLM Grid������Ч,����,������ó�True,TLM NRI����ᱨ��!)
          self._config['HYBRID']=True                 �Ƿ����ģ�ķֲ�ͼ��
          self._config['HYBRID_EXPR']="sqrt(x*x+y*y)" ����ģ�ļ��㺯��
          self._config['HYBRID_LIST']=["EX,EY"]       ����Ҫ����ģ�ı���,���֧��"EX,EY,EZ",���Ի��ƶ��,�ö��Ÿ���
                                                      (�˴�ֻ������ͬһ�ּ��㷽ʽ,����["EX,EY","EX,EY,EZ"]�ǲ������
          self._config['SHOW_MEDIUM']=True            �Ƿ��ڻ��Ƶ�ͼ������ʾ���ʷֲ�
          self._config['ANI']=True                    �Ƿ����ɶ�ά����ά�Ķ���
          self._config['DO_EX']=True                  �Ƿ����ɶ�Ӧ��ͼ��
          self._config['DO_EY']=True                  (��ѡ���twotlme.in�е�GEN_��Ӧ,��ѡ�������Ӧ��GEN_���ó�1ʱ����Ч
          self._config['DO_EZ']=True                  ����,���GEN_EY=0,��ô���۴˴�DO_EY�Ƿ�ΪTrue,���ᱻTLM.py�Զ�����,
          self._config['DO_HX']=True                  �Ӷ���������EY��ͼ��Ͷ���)
          self._config['DO_HY']=True
          self._config['DO_HZ']=True 

     4.  Ƶ����㹤��:
          �ڵõ����е�������ݺ�,����˫��tlm_dft.py,������Ҫ�����X,Z����,���ɵõ���Ӧ��Ƶ�����

     5.  �������õĹ���:
          ���������Ҫ������TLM����һ��ʱ��֮��������ݵ�Ч��,�Ϳ���˫��gen_ani.bat�õ���Ӧ�Ķ�ά����ά����,          
          clean.bat��������������е���������ļ���ͼ���ļ�(���ǰ��Ҫ��ȷ��),���յĶ����ļ����ᱻɾ��

3.  twotlme.in�����ļ��޸�˵��:
    TLM����������˳����������ļ��е�����:
    SX,SY,SZ,                 ģ����ά�ռ����ʼ����
    ENDX,ENDY,ENDZ,           ģ����ά�ռ����ֹ����
    X,Y,Z,                    X��Z�����壬Yָ������ģ���Yƽ��
    U,V,W,H,ER,UR,SGM,        �������������ERΪ��ά�ռ��Ĭ��ERֵ
    NT,BL,                    NTΪģ����� BLΪ�߽����
    SPLIT,                    SPLIT=1 ��T�ָ�����ļ�, SPLIT=0 ���ָ�,Ĭ��Ϊ1,Ϊʹ������ȷ����,��ȷ��SPLIT=1
    SIDE1,SIDE2,              ��������ĸߺͿ�
    ER1,ER2,                  ����ER����

    NANGLE,                   ��������ƫ�ƽǶ�(��Z���Ž�,ȡֵ��ΧΪ(0,180) Ĭ��ֵ90Ϊ����б
                              (�������Ѳ���ʹ��,��������ֵ�Գ��򲻻����κ�Ӱ��,���鱣��ΪĬ��ֵ90)

    GTYPE,                    �����������״:
                              0Ϊ����, 1Ϊ������Բ(��Բ), 2Ϊ����������(����Ϊ������ʽ,ER����)
                              3Ϊ����״������(����4x4��16��������Ƴ�������,����Ҫ��������,����
                              ȷ��SIDE1/SIDE2=SQRT(3)/2,ER1�����ѿ�϶,ER2������е�������)

    COL,                      ��GTYPEΪ3(Ҫ�����״������ʱ),��ֵΪ4*SIDE2��С���� 
                              ���������,��ֵΪ�������������

    FRE,NORMAL_FRE,           FRE����ʹ��,NORMAL_FREΪ����Դ�Ĺ�һ��Ƶ��

    W0,                       ��˹�ֲ�����(��˹�ֲ�����Ĭ�Ϲر�,�������ʹ��Visual Fortran��Դ����,�޸ĵ�447��
                              !              IVB(III,I,J,K)=GAUSS(I,J,K)*IVB(III,I,J,K)
                              ��ע�ͷ�!ȥ��,���ɴ򿪸�˹�ֲ�

    PN,                       PN=0ʱ: ʹ��PSX,PSZ,PENDZ�Զ���������ֱ��,ֱ����תPANGLE�Ƕ�
                              PN>0ʱ: ʹ�������ļ���X,Z������Ϊ�����
                              (X,Z�����twotlme.in�ļ��ĵڶ��п�ʼ,ÿһ������һ��������λ��)

    PSX,PSZ,PENDZ,PANGLE,     PSX,PSZΪ�������忪ʼλ��,PENDZΪ�������������Z����
                              PANGLEΪ��������ֱ����Z���Ž�(�ǶȲ���)
                              (���ϲ�������PN=0ʱ��Ч)

    LN,                       twotlme.in�ļ���ָ����Ƶ�������,����Ƶ�������������������֮��

    BP,                       ����Z����Ϊ���ձ߽�,�ò���Ĭ�Ϲر�,�������ʹ��Visual Fortran��Դ����,�޸ĵ�584��
                              !IVB(2,I,J,K)=0
                              ��ע�ͷ�!ȥ��,���ɴ򿪸ù���

    SCALE                     ���Ϊ0,�����PN��ȡֵȷ��������,��������һ��������,��λ����SCALEȷ��
                              (������������=���ʿ��/SCALE)

    GEN_EX, GEN_EY, GEN_EZ, GEN_HX, GEN_HY, GEN_HZ, GEN_FEX, GEN_FEY, GEN_FEZ, GEN_FHX, GEN_FHY, GEN_FHZ
                              �����Ƿ����ɶ�Ӧ������(1:����,0:������)

    NORMALIZE                 ���Ϊ1,���������ݽ��й�һ������(���������ݳ������ݼ������ֵ)