package bird.wallpaper;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

public class CopyFile
{

	public static void main(String[] args)
	{
		File from = new File("C:\\Documents\\javaws\\Lanuch\\assets\\fairyland.xml");
		File to = new File("X:\\6575\\v2\\alps\\packages\\apps\\Launcher2\\assets/fairyland.xml");

	}

	public static void copyText(File from, File to)
	{
		try
		{
			FileReader fileReader = new FileReader(from);
			BufferedReader br = new BufferedReader(fileReader);
			FileWriter fileWriter = new FileWriter(to);
			String line;
			while ((line = br.readLine()) != null)
			{
				fileWriter.write(line + "\n");
			}
			fileReader.close();
			br.close();
			fileWriter.close();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

	static byte[] b = new byte[1024];
	
	public static void copyBinary(File from, File toDir)
	{
		if (!toDir.exists())
		{
			toDir.mkdirs();
		}
		try
		{
			FileInputStream fileInputStream = new FileInputStream(from);
			FileOutputStream fileOutputStream = new FileOutputStream(new File(toDir, from.getName()));
			int len;
			while ((len = fileInputStream.read(b)) != -1)
			{
				fileOutputStream.write(b, 0, len);
			}
			fileOutputStream.close();
			fileInputStream.close();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
	

	
	public static void BinaryFileCut(File from, File to)
	{
		try
		{
			FileInputStream fileInputStream = new FileInputStream(from);
			FileOutputStream fileOutputStream = new FileOutputStream(to);
			int len;
			while ((len = fileInputStream.read(b)) != -1)
			{
				fileOutputStream.write(b, 0, len);
			}
			fileOutputStream.close();
			fileInputStream.close();
			from.delete();
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}

}
