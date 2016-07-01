package bird.wallpaper;

import java.util.*;
import java.io.*;

import org.dom4j.*;
import org.dom4j.io.*;

import bird.BirdEnvironment;
import bird.FileValidate;

public class FrameworkWallaperHandler
{

	private String foldName;
	private File dstXMLFile;
	private File wallpaperFile;
	private File projectFile;
	private File wallpaperToFile;

	public FrameworkWallaperHandler(String foldName)
	{
		this.foldName = foldName;
		projectFile = BirdEnvironment.getProjectFile();
		dstXMLFile = new File(projectFile, "frameworks/base/core/res/res/values/public.xml");
		wallpaperFile = findWallpapersFile();
		wallpaperToFile = new File(projectFile, "frameworks/base/core/res/res/drawable-nodpi");

		System.out.println("FrameworkWallaperHandler dstXMLFile=" + dstXMLFile);
		System.out.println("FrameworkWallaperHandler wallpaperFile=" + wallpaperFile);
		System.out.println("FrameworkWallaperHandler wallpaperToFile=" + wallpaperToFile);

	}

	private File findWallpapersFile()
	{
		File rootFold = new File(projectFile, "bird/wallpaper/framework_wallpaper/" + foldName);
		if (rootFold != null && !rootFold.exists())
		{
			return null;
		}
		for (File f : rootFold.listFiles())
		{
			if (f.isDirectory())
			{
				String[] fileList = f.list();

				if (fileList.length > 0)
				{
					for (String fileName : fileList)
					{
						if (FileValidate.isImageFile(fileName))
						{
							wallpaperFile = f;
						}
					}
				}
			}
			else if (FileValidate.isImageFile(f))
			{
				wallpaperFile = rootFold;
			}
		}
		return wallpaperFile;
	}

	public static void main(String[] args)
	{
		System.out.println("------------debug----------");
		try
		{
			if (args.length > 0)
			{
				String foldName = args[0];
				System.out.println("main  foldName=" + foldName);
				FrameworkWallaperHandler handler = new FrameworkWallaperHandler(foldName);
				handler.writePublicXml();
				handler.copyFile();
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
		System.out.println("------------debug ok----------");

	}

	private void copyFile()
	{
		for (File wallpaper : wallpaperFile.listFiles())
		{
			if (FileValidate.isImageFile(wallpaper.getName()))
			{
				CopyFile.copyBinary(wallpaper, wallpaperToFile);
			}
		}
	}

	public void writePublicXml() throws Exception
	{
		ArrayList<String> imgNameList = new ArrayList<String>();

		System.out.println(wallpaperFile);
		String[] listFiles = wallpaperFile.list();

		for (String str : listFiles)
		{
			if (FileValidate.isImageFile(str))
			{
				System.out.println(str);
				String imgName = str.subSequence(0, str.length() - 4).toString();
				System.out.println(imgName);
				imgNameList.add(imgName);
			}
		}
		int count = 0;
		SAXReader reader = new SAXReader();
		Document doc = reader.read(dstXMLFile); // ¼ÓÔØxmlÎÄ¼þ
		for (String imaName : imgNameList)
		{
			System.out.println("imaName:" + imaName);
			Element ele = (Element) doc.selectSingleNode("//public[@name='" + imaName + "']");
			if (ele != null)
			{
				System.out.println("Element " + ele.getName() +" existed !!! ");
			}
			else
			{
				System.out.println("Element=" + ele);
				count++;
				Element rootElement = doc.getRootElement();
				Element element = rootElement.addElement("public");
				element.addAttribute("type", "drawable");
				element.addAttribute("name", imaName.toString());
			}
		}
		System.out.println(count);
		if (count > 0)
		{
			XMLWriter writer = new XMLWriter(new OutputStreamWriter(new FileOutputStream(dstXMLFile)), OutputFormat.createPrettyPrint());
			writer.write(doc);
			writer.close();
		}
	}

}