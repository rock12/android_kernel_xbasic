package bird;

import java.io.File;

public class FileValidate
{
	public static boolean isImageFile(String name)
	{
		if (name.endsWith(".jpg") || name.endsWith(".png"))
		{
			return true;
		}
		return false;
	}

	public static boolean isImageFile(File f)
	{
		String name = f.getName();
		if (name.endsWith(".jpg") || name.endsWith(".png"))
		{
			return true;
		}
		return false;
	}
}
