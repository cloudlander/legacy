import java.io.*;
import java.awt.*;
import java.awt.event .*;

public class wsuperFile extends Frame implements ActionListener
{
	Button cutter,reviver,exit;
	public static void main(String[] args)
	{
		new wsuperFile ();
	}
	wsuperFile()
	{
		super("superFile");

		setLayout(new FlowLayout ());
		cutter=new Button ("文件切割");
		reviver=new Button ("文件恢复");
		exit=new Button ("退出");

		add(cutter);  add(reviver);  add(exit);

		cutter.addActionListener (this);
		reviver.addActionListener (this);
		exit.addActionListener (this);

		setSize(new Dimension (200,100));
		setVisible(true);
	}
	public void actionPerformed(ActionEvent e)
	{
		dispose();
		if(e.getSource ()==cutter)
			new FileCutter();
		else if(e.getSource ()==reviver)
			new FileReviver();
		else
		{
			System.exit(0);
		}
	}
}

class Myframe extends Frame implements ActionListener,ItemListener
{
	Label label1,label2,process;
	Label ls,ld;
	Button source,dest,start,Return;
	Checkbox fp;
	FileDialog fd,fd1;
	Dialog decide;
	Button ok;
	String fpath1,fpath2;
	TextField tf;
	GridBagLayout gbLayout=new GridBagLayout();
	GridBagConstraints gbc=new GridBagConstraints ();

	Myframe(String str)
	{
		super(str);
		process=new Label("进度");
		source=new Button ("浏览...");
		dest=new Button ("浏览...");
		start=new Button ("开始");
		Return=new Button("返回");
		label1=new Label();
		label2=new Label ();
		ls=new Label ("F:\\       ");
		ld=new Label ("F:\\temp   ");
		fp=new Checkbox ();
		decide=new Dialog (this,true);
		ok=new Button("确定");
		decide.setSize (new Dimension (90,60));
		decide.add("Center",ok);
		setSize(new Dimension (320,200));

		setLayout(gbLayout);

		gbc.gridx=1;  gbc.gridy=1;
		gbc.gridheight=1; gbc.gridwidth =1;
		gbc.anchor=GridBagConstraints.SOUTHEAST ;
		gbc.insets=new Insets(2,5,1,5);
		gbLayout.setConstraints (label1,gbc);
		add(label1);

		gbc.gridx=2;  gbc.gridy=1;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (ls,gbc);
		add(ls);

		gbc.gridx=3;  gbc.gridy=1;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (source,gbc);
		add(source);

		gbc.gridx=1;  gbc.gridy=2;
		gbc.gridheight=1; gbc.gridwidth =1;
		gbc.anchor=GridBagConstraints.SOUTHEAST ;
		gbc.insets=new Insets(2,5,1,5);
		gbLayout.setConstraints (label2,gbc);
		add(label2);

		gbc.gridx=2;  gbc.gridy=2;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (ld,gbc);
		add(ld);

		gbc.gridx=3;  gbc.gridy=2;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (dest,gbc);
		add(dest);

		gbc.gridx=1;  gbc.gridy=4;
		gbc.gridheight=1; gbc.gridwidth =1;
		gbc.anchor=GridBagConstraints.SOUTHEAST ;
		gbc.insets=new Insets(2,5,1,5);
		gbLayout.setConstraints (process,gbc);
		add(process);

		gbc.gridx=2;  gbc.gridy=4;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (start,gbc);
		add(start);

		gbc.gridx=3;  gbc.gridy=4;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (Return,gbc);
		add(Return);

		gbc.gridx=1;  gbc.gridy=5;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (fp,gbc);
		add(fp);

		source.addActionListener (this);
		dest.addActionListener (this);
		start.addActionListener (this);
		Return.addActionListener (this);
		fp.addItemListener (this);
		ok.addActionListener (this);
	}

	public int toInt(String s)
	{
		int num=0,i;
		for(i=0;i<s.length ();i++)
			num=num*10+(s.charAt (i)-'0');
		return num;
	}
	public void actionPerformed(ActionEvent e)
	{
	}
	public void itemStateChanged(ItemEvent e)
	{
	}
}

class FileCutter extends Myframe
{
	Label label3;
	CheckboxGroup size;
	Checkbox bt,kbt;
	TextField tf;

	FileCutter()
	{
		super("FileCutter");
		label3=new Label ("块字节数:");
		size=new CheckboxGroup ();
		bt=new Checkbox("×B",true,size);
		kbt=new Checkbox("×KB",false,size);
		tf=new TextField (Integer.toString (1000));

		size.setSelectedCheckbox(bt);
		label1.setText ("源文件:");
		label2.setText ("目标目录:");
		fp.setLabel ("分割到软盘");

		gbc.gridx=1;  gbc.gridy=3;
		gbc.gridheight=1; gbc.gridwidth =1;
		gbc.anchor=GridBagConstraints.SOUTHEAST ;
		gbc.insets=new Insets(2,5,1,5);
		gbLayout.setConstraints (label3,gbc);
		add(label3);

		gbc.gridx=2;  gbc.gridy=3;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (tf,gbc);
		add(tf);

		gbc.gridx=3;  gbc.gridy=3;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (bt,gbc);
		add(bt);

		gbc.gridx=4;  gbc.gridy=3;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (kbt,gbc);
		add(kbt);

		setVisible(true);
	}
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource ()==source)
		{
			fd=new FileDialog (this,"打开文件",FileDialog.LOAD );
			fd.setDirectory ("F:\\temp");
			fd.show();
			fpath1=fd.getDirectory ()+fd.getFile ();
			ls.setText ("...\\"+fd.getFile ());
		}
		else if(e.getSource ()==dest){
			fd1=new FileDialog (this,"保存文件",FileDialog.SAVE  );
			fd1.setDirectory ("F:\\temp");
			fd1.setFile ("Defaltcut1.spf");
			fd1.show();
			fpath2=fd1.getDirectory ();
			ld.setText (fpath2);
		}
		else if(e.getSource ()==Return)
		{
			dispose();
			new wsuperFile();
		}else if(e.getSource ()==ok)
		{
			decide.dispose();
		}else
		{
			filecutter();
		}
	}
	public void itemStateChanged(ItemEvent e)
	{
		if(fp.getState ())
		{
			tf.setText ("1380");
			tf.setEditable(false);
			size.setSelectedCheckbox(kbt);
			decide.setTitle ("请放入软盘");
			decide.show();
		}
		else tf.setEditable (true);
	}
	void filecutter()
	{
		int chi=0;
		int j=1;
		int block;
		boolean sta=fp.getState ();
		start.setEnabled (false);
		Return.setEnabled (false);
        source.setEnabled (false);
		dest.setEnabled (false);
		fp.setEnabled(false);

		File srcf=new File(fpath1);
		if(bt.getState ())
			block=toInt(tf.getText());
		else
			block=toInt(tf.getText())*1000;
		try{
			FileInputStream isrcf=new FileInputStream (srcf);
		    if(srcf.exists ())
		    {
				 process.setText ("进行中");
			     FileOutputStream df=new FileOutputStream (fpath2+"cut1.spf");
			     while(true)
			     {
				       for(int i=1;i<=block&&(chi=isrcf.read ())!=-1;i++)
					        df.write (chi);
				       df.close ();
					   if(chi==-1) break;
					   if(sta)
					   {
					       decide.setTitle ("请更换软盘");
					       decide.show();
					   }
				       df=new FileOutputStream (fpath2+"cut"+Integer.toString (++j)+".spf");
			     }
				 isrcf.close ();
				 process.setText ("完成");
				 source.setEnabled (true);
				 dest.setEnabled (true);
				 start.setEnabled (true);
				 Return.setEnabled (true);
				 fp.setEnabled (true);
			 }
		}
		catch(IOException e){
			System.err.println(e);
		}
	}
}

class FileReviver extends Myframe
{
	FileReviver()
	{
		super("FileReviver");

		label1.setText("源目录:");
		label2.setText ("保存为:");
		Label label4=new Label ("输入文件数:");
		fp.setLabel ("从软盘恢复");
        tf=new TextField (5);

		gbc.gridx=3;  gbc.gridy=5;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (tf,gbc);
		add(tf);

		gbc.gridx=2;  gbc.gridy=5;
		gbc.gridheight =1;  gbc.gridwidth =1;
		gbc.insets =new Insets(2,5,1,5);
		gbLayout.setConstraints (label4,gbc);
		add(label4);

		setVisible(true);
	}
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource ()==source)
		{
			fd=new FileDialog (this,"打开文件",FileDialog.LOAD );
			fd.setDirectory ("F:\\temp");
			fd.show();
			fpath1=fd.getDirectory ();
			ls.setText (fpath1);
		}
		else if(e.getSource ()==dest){
			fd1=new FileDialog (this,"保存文件",FileDialog.SAVE  );
			fd1.setDirectory ("F:\\temp");
			fd1.show();
			fpath2=fd1.getDirectory ()+fd1.getFile ();
			ld.setText ("...\\"+fd1.getFile ());
		}
		else if(e.getSource ()==Return)
		{
			dispose();
			new wsuperFile();
		}else if(e.getSource ()==ok)
		{

			decide.dispose ();
		}else
		{
			filereviver();
		}
	}
	public void itemStateChanged(ItemEvent e)
	{
		if(fp.getState ())
		{
			decide.setTitle ("请放入软盘");
			decide.show();
		}
	}
	public void filereviver()
	{
		int chi;
		int i;
		int n=0;
		File dir=new File(fpath1);
		boolean sta=fp.getState ();
		start.setEnabled(false);
		Return.setEnabled(false);
        source.setEnabled(false);
		dest.setEnabled(false);
		fp.setEnabled(false);
		tf.setEnabled (false);
		try
		{
			if(dir.isDirectory ()) n=(dir.list()).length;
			if(sta) n=toInt(tf.getText ());
			process.setText ("进行中");
			FileOutputStream df=new FileOutputStream (fpath2);
			for(i=1;i<=n;i++){
			   FileInputStream srcf=new FileInputStream (fpath1+"cut"+Integer.toString (i)+".spf");

			   while((chi=srcf.read ())!=-1)
				  df.write (chi);
			   srcf.close ();
			   if(i<n&&sta)
			   {
			       decide.setTitle ("请更换软盘");
			       decide.show();
			   }
			}
			df.close ();
		    process.setText ("完成");
			source.setEnabled (true);
		    dest.setEnabled (true);
			start.setEnabled (true);
			Return.setEnabled (true);
			fp.setEnabled (true);
			tf.setEnabled (true);
		}
		catch(IOException e)
		{
			System.err.println (e);
		}
	}
}
