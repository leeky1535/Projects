package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.EnvironmentCheckDao;
import com.ssafy.dto.EnvironmentCheckDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class EnvironmentCheckDaoImpl implements EnvironmentCheckDao {
	private final String DRIVER = "com.mysql.cj.jdbc.Driver";

	private static EnvironmentCheckDao EnvironmentCheckDao;

	private EnvironmentCheckDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static EnvironmentCheckDao getEnvironmentCheckDao() {
		if (EnvironmentCheckDao == null)
			EnvironmentCheckDao = new EnvironmentCheckDaoImpl();
		return EnvironmentCheckDao;
	}

	@Override
	public List<EnvironmentCheckDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<EnvironmentCheckDto> EnvironmentCheckDtoList = new ArrayList<EnvironmentCheckDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select * " + "from environmentcheck";
			pstmt = conn.prepareStatement(sql);

			rs = pstmt.executeQuery();
			while (rs.next()) {
				EnvironmentCheckDto EnvironmentCheckDto = new EnvironmentCheckDto(rs.getString("name"),
						rs.getString("check_date"), rs.getString("to_be_removed"), rs.getString("issue"),
						rs.getString("street_address"), rs.getString("address"), rs.getString("dong_code"));
				EnvironmentCheckDtoList.add(EnvironmentCheckDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return EnvironmentCheckDtoList;
	}

	@Override
	public List<EnvironmentCheckDto> searchByDongcodeIssue(String dongCode, String issue) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<EnvironmentCheckDto> EnvironmentCheckDtoList = new ArrayList<EnvironmentCheckDto>();
		try {
			conn = DBConnection.getConnection();
			StringBuilder sql = new StringBuilder();
			sql.append("select * " + "from environmentcheck where");
			if (dongCode != null) {
				sql.append("dong_code = ?");
			}
			if (issue != null) {
				sql.append("issue = ?");
			}
			pstmt = conn.prepareStatement(sql.toString());
			if (dongCode != null && issue == null) {
				pstmt.setString(1, dongCode);
			}
			if (dongCode == null && issue != null) {
				pstmt.setString(1, issue);
			}
			if (dongCode != null && issue != null) {
				pstmt.setString(1, dongCode);
				pstmt.setString(2, issue);
			}
			rs = pstmt.executeQuery();
			while (rs.next()) {
				EnvironmentCheckDto EnvironmentCheckDto = new EnvironmentCheckDto(rs.getString("name"),
						rs.getString("check_date"), rs.getString("to_be_removed"), rs.getString("issue"),
						rs.getString("street_address"), rs.getString("address"), rs.getString("dong_code"));
				EnvironmentCheckDtoList.add(EnvironmentCheckDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return EnvironmentCheckDtoList;
	}

}
