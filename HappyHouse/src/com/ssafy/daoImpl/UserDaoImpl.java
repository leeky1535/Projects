package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.UserDao;
import com.ssafy.dto.UserDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class UserDaoImpl implements UserDao {

	private final String DRIVER = "com.mysql.cj.jdbc.Driver";
	private final String URL = "jdbc:mysql://127.0.0.1:3306/happyhouse?serverTimezone=UTC&useUniCode=yes&characterEncoding=UTF-8";
	private final String DB_ID = "ssafy";
	private final String DB_PASS = "ssafy";

	private static UserDao userDao;

	private UserDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static UserDao getUserDao() {
		if (userDao == null)
			userDao = new UserDaoImpl();
		return userDao;
	}

	@Override
	public void register(UserDto userDto) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			conn.setAutoCommit(false);
			StringBuilder sql = new StringBuilder();
			sql.append(
					"insert into user (userid, password, name, email, phone, address, addressdetail, favoritedongcode)\r\n"
							+ "values (?,?,?,?,?,?,?,?)");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setString(1, userDto.getUserId());
			pstmt.setString(2, userDto.getPassword());
			pstmt.setString(3, userDto.getName());
			pstmt.setString(4, userDto.getEmail());
			pstmt.setString(5, userDto.getPhone());
			pstmt.setString(6, userDto.getAddress());
			pstmt.setString(7, userDto.getAddressDetail());
			pstmt.setString(8, userDto.getFavoriteDongCode());

			pstmt.executeUpdate();
			System.out.println("userId: " + userDto.getUserId() + " 등록성공!!!");
			conn.commit();
		} catch (SQLException e) {
			e.printStackTrace();
			try {
				conn.rollback();
			} catch (SQLException e1) {
				e1.printStackTrace();
			}
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

	}

	@Override
	public UserDto searchById(String userId) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		UserDto user = null;
		try {
			conn = DBConnection.getConnection();
			String sql = "select userid, password, name, email, phone, address, addressdetail, favoritedongcode from user where userid = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userId);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				user = new UserDto(rs.getString("userid"), rs.getString("password"), rs.getString("name"),
						rs.getString("email"), rs.getString("phone"), rs.getString("address"),
						rs.getString("addressdetail"), rs.getString("favoritedongcode"));
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return user;
	}

	@Override
	public List<UserDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<UserDto> listUserDto = new ArrayList<UserDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select userid, password, name, email, phone, address, addressdetail, favoritedongcode \n";
			sql += "from user";
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				UserDto user = new UserDto(rs.getString("userid"), rs.getString("password"), rs.getString("name"),
						rs.getString("email"), rs.getString("phone"), rs.getString("address"),
						rs.getString("addressdetail"), rs.getString("favoritedongcode"));
				listUserDto.add(user);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return listUserDto;
	}

	@Override
	public void updateUserInfo(UserDto userDto) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DBConnection.getConnection();
			String sql = "update user \n";
			sql += "set password = ? \n,";
			sql += "name = ? \n,";
			sql += "email = ? \n,";
			sql += "phone = ? \n,";
			sql += "address = ? \n,";
			sql += "addressdetail = ? \n,";
			sql += "favoritedongcode = ? \n";
			sql += "where userid = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userDto.getPassword());
			pstmt.setString(2, userDto.getName());
			pstmt.setString(3, userDto.getEmail());
			pstmt.setString(4, userDto.getPhone());
			pstmt.setString(5, userDto.getAddress());
			pstmt.setString(6, userDto.getAddressDetail());
			pstmt.setString(7, userDto.getFavoriteDongCode());
			pstmt.setString(8, userDto.getUserId());

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 변경성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(conn, pstmt);
		}

	}

	@Override
	public void deleteUser(String userId) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DBConnection.getConnection();
			String sql = "delete from user \n";
			sql += "where userid = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userId);

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 삭제성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(conn, pstmt);
		}

	}

}
