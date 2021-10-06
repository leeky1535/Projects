package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.InterestedDao;
import com.ssafy.dto.InterestedDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class InterestedDaoImpl implements InterestedDao {
	private final String DRIVER = "com.mysql.cj.jdbc.Driver";
	private final String URL = "jdbc:mysql://127.0.0.1:3306/happyhouse?serverTimezone=UTC&useUniCode=yes&characterEncoding=UTF-8";
	private final String DB_ID = "ssafy";
	private final String DB_PASS = "ssafy";

	private static InterestedDao interestedDao;

	private InterestedDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static InterestedDao getInterestedDao() {
		if (interestedDao == null)
			interestedDao = new InterestedDaoImpl();
		return interestedDao;
	}

	@Override
	public void register(String userId, String dong) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			conn.setAutoCommit(false);
			StringBuilder sql = new StringBuilder();
			sql.append("insert into interested (userid, dongcode)\r\n"
					+ "values (?, (select dongcode from dongcode where dong = ? limit 1))");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setString(1, userId);
			pstmt.setString(2, dong);
			pstmt.executeUpdate();
			System.out.println("userId: " + userId + " 의 관심지역 " + dong + " 등록성공!!!");
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
	public void delete(String userId, String dong) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = DriverManager.getConnection(URL, DB_ID, DB_PASS);
			conn.setAutoCommit(false);
			StringBuilder sql = new StringBuilder();
			sql.append(
					"delete from interested where userid = ? and dongcode = (select dongcode from dongcode where dong = ? limit 1)");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setString(1, userId);
			pstmt.setString(2, dong);
			pstmt.executeUpdate();
			System.out.println("userId: " + userId + " 의 관심지역 " + dong + " 삭제성공!!!");
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
	public List<InterestedDto> searchById(String userId) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<InterestedDto> interestedList = new ArrayList<InterestedDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select *\n" + "from interested" + " where userid = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, userId);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				InterestedDto interestedDto = new InterestedDto(rs.getString("userid"), rs.getString("dongcode"));

				interestedList.add(interestedDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return interestedList;

	}
}
