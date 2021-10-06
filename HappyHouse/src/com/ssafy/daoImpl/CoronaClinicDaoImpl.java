package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.CoronaClinicDao;
import com.ssafy.dto.CoronaClinicDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class CoronaClinicDaoImpl implements CoronaClinicDao {

	private final String DRIVER = "com.mysql.cj.jdbc.Driver";

	private static CoronaClinicDao coronaClinicDao;

	private CoronaClinicDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static CoronaClinicDao getCoronaClinicDao() {
		if (coronaClinicDao == null)
			coronaClinicDao = new CoronaClinicDaoImpl();
		return coronaClinicDao;
	}

	@Override
	public List<CoronaClinicDto> searchByGugun(String gugun) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<CoronaClinicDto> coronaClinicDtoList = new ArrayList<CoronaClinicDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select date_effective, gugun_code, name, address, business_hours_holiday, phone "
					+ "from houseinfo " + "where gugun_code = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, gugun);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				CoronaClinicDto coronaClinicDto = new CoronaClinicDto(rs.getString("date_effective"),
						rs.getString("gugun_code"), rs.getString("name"), rs.getString("address"),
						rs.getString("business_hours_weekdays"), rs.getString("business_hours_saturday"),
						rs.getString("business_hours_holiday"), rs.getString("phone"));
				coronaClinicDtoList.add(coronaClinicDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return coronaClinicDtoList;
	}

	@Override
	public List<CoronaClinicDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<CoronaClinicDto> coronaClinicDtoList = new ArrayList<CoronaClinicDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select date_effective, gugun_code, name, address, business_hours_holiday, phone "
					+ "from houseinfo";
			pstmt = conn.prepareStatement(sql);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				CoronaClinicDto coronaClinicDto = new CoronaClinicDto(rs.getString("date_effective"),
						rs.getString("gugun_code"), rs.getString("name"), rs.getString("address"),
						rs.getString("business_hours_weekdays"), rs.getString("business_hours_saturday"),
						rs.getString("business_hours_holiday"), rs.getString("phone"));
				coronaClinicDtoList.add(coronaClinicDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return coronaClinicDtoList;
	}

}
