package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.UserDao;
import com.ssafy.dto.UserDto;
import com.ssafy.util.DBUtil;

public class UserDaoImpl implements UserDao {

	private static UserDao userDao = new UserDaoImpl();
	private DBUtil dbUtil;

	private UserDaoImpl() {
		dbUtil = DBUtil.getInstance();
	}

	public static UserDao getUserDao() {
		return userDao;
	}

	@Override
	public void register(UserDto userDto) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			conn.setAutoCommit(false);
			StringBuilder sql = new StringBuilder();
			sql.append("insert into user (username, password, name, email, phone, address, address_detail)\r\n"
					+ "values (?,?,?,?,?,?,?)");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setString(1, userDto.getUsername());
			pstmt.setString(2, userDto.getPassword());
			pstmt.setString(3, userDto.getName());
			pstmt.setString(4, userDto.getEmail());
			pstmt.setString(5, userDto.getPhone());
			pstmt.setString(6, userDto.getAddress());
			pstmt.setString(7, userDto.getAddressDetail());

			pstmt.executeUpdate();
			System.out.println("userId: " + userDto.getUsername() + " 등록성공!!!");
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
	public UserDto searchById(String userId) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		UserDto user = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "select username, password, name, email, phone, address, address_detail from user where username = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userId);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				user = new UserDto(rs.getString("username"), rs.getString("password"), rs.getString("name"),
						rs.getString("email"), rs.getString("phone"), rs.getString("address"),
						rs.getString("address_detail"));
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return user;
	}

	@Override
	public List<UserDto> searchAll() throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<UserDto> listUserDto = new ArrayList<UserDto>();
		try {
			conn = dbUtil.getConnection();
			String sql = "select username, password, name, email, phone, address, address_detail \n";
			sql += "from user";
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				UserDto user = new UserDto(rs.getString("username"), rs.getString("password"), rs.getString("name"),
						rs.getString("email"), rs.getString("phone"), rs.getString("address"),
						rs.getString("address_detail"));
				listUserDto.add(user);
			}
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return listUserDto;
	}

	@Override
	public void updateUserInfo(UserDto userDto) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "update user \n";
			sql += "set password = ? \n,";
			sql += "name = ? \n,";
			sql += "email = ? \n,";
			sql += "phone = ? \n,";
			sql += "address = ? \n,";
			sql += "address_detail = ? \n";
			sql += "where username = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userDto.getPassword());
			pstmt.setString(2, userDto.getName());
			pstmt.setString(3, userDto.getEmail());
			pstmt.setString(4, userDto.getPhone());
			pstmt.setString(5, userDto.getAddress());
			pstmt.setString(6, userDto.getAddressDetail());
			pstmt.setString(7, userDto.getUsername());

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 변경성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(conn, pstmt);
		}

	}

	@Override
	public void deleteUser(String userId) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			String sql = "delete from user \n";
			sql += "where username = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userId);

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 삭제성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(conn, pstmt);
		}

	}

	@Override
	public int usernameCheck(String username) throws Exception {
		int cnt = 1;
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		try {
			conn = dbUtil.getConnection();
			StringBuilder loginUser = new StringBuilder();
			loginUser.append("select count(username) \n");
			loginUser.append("from user \n");
			loginUser.append("where username = ?");
			pstmt = conn.prepareStatement(loginUser.toString());
			pstmt.setString(1, username);
			rs = pstmt.executeQuery();
			rs.next();
			cnt = rs.getInt(1);
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return cnt;
	}

	@Override
	public UserDto login(String username, String password) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		UserDto userDto = null;
		try {
			conn = dbUtil.getConnection();
			StringBuilder loginUser = new StringBuilder();
			loginUser.append("select username, name, email, address, address_detail, phone \n");
			loginUser.append("from user \n");
			loginUser.append("where username = ? and password = ?");
			pstmt = conn.prepareStatement(loginUser.toString());
			pstmt.setString(1, username);
			pstmt.setString(2, password);
			rs = pstmt.executeQuery();
			if (rs.next()) {
				userDto = new UserDto();
				userDto.setUsername(rs.getString("username"));
				userDto.setName(rs.getString("name"));
				userDto.setEmail(rs.getString("email"));
				userDto.setAddress(rs.getString("address"));
				userDto.setAddressDetail(rs.getString("address_detail"));
				userDto.setPhone(rs.getString("phone"));
			}
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return userDto;
	}

}
