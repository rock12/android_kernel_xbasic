package bird;

import java.io.File;

public class BirdEnvironment
{

	private static File parFile;

	public static File getProjectFile()
	{
		File environment = new File(System.getProperty("user.dir"));
		return getProjectFile(environment);
	}

	private static File getProjectFile(File f)
	{
		File floatUp = floatUp(f);
		System.out.println("BirdEnvironment  ProjectFile=" + floatUp);
		return floatUp;
	}

	private static File floatUp(File f)
	{
		boolean flagFind = false;
		;
		File parentFile = f.getParentFile();
		a: while (parentFile != null)
		{
			String[] list = parentFile.list();
			for (int i = 0; i < list.length; i++)
			{
				String fName = list[i];
				if (fName.equals("makeBird"))
				{
					parFile = parentFile;
					flagFind = true;
					break a;
				}
			}
			if (!flagFind)
			{
				floatUp(parentFile);
			}
			break;
		}
		return parFile;
	}
}
