package com.ssafy.util;

import java.sql.Connection;
import java.sql.SQLException;

import javax.naming.*;
import javax.sql.DataSource;

public class DBUtil {
	private static DBUtil instance = new DBUtil();

	private DBUtil() {}

	public static DBUtil getInstance() {
		return instance;
	}

	public Connection getConnection() throws SQLException {
		try {
			Context context = new InitialContext();
			Context root = (Context) context.lookup("java:comp/env");
			DataSource ds = (DataSource) root.lookup("jdbc/ssafy");
			return ds.getConnection();
		} catch (NamingException e) {
			e.printStackTrace();
		}
		return null;
	}

	public void close(AutoCloseable... closeables) {
		for (AutoCloseable c : closeables) {
			if (c != null) {
				try {
					c.close();
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}
}
