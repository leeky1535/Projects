package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

import com.ssafy.dao.SanggwonDao;
import com.ssafy.dto.SanggwonDto;
import com.ssafy.util.DBClose;
import com.ssafy.util.DBConnection;

public class SanggwonDaoImpl implements SanggwonDao {
	private final String DRIVER = "com.mysql.cj.jdbc.Driver";

	private static SanggwonDao sanggwonDao;

	private SanggwonDaoImpl() {
		try {
			Class.forName(DRIVER);
			System.out.println("드라이버 로딩 성공");
		} catch (ClassNotFoundException e) {
			e.printStackTrace();
			System.out.println("드라이버 로딩 실패");
		}
	}

	public static SanggwonDao getSanggwonDao() {
		if (sanggwonDao == null)
			sanggwonDao = new SanggwonDaoImpl();
		return sanggwonDao;
	}

	@Override
	public List<SanggwonDto> searchAll() {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<SanggwonDto> SanggwonDtoList = new ArrayList<SanggwonDto>();
		try {
			conn = DBConnection.getConnection();
			String sql = "select * " + "from sanggwon";
			pstmt = conn.prepareStatement(sql);

			rs = pstmt.executeQuery();
			while (rs.next()) {
				SanggwonDto SanggwonDto = new SanggwonDto(rs.getInt("no"), rs.getString("name"), rs.getString("branch"),
						rs.getString("business_category"), rs.getString("dong_code"), rs.getString("address"),
						rs.getString("building_name"), rs.getString("street_address"), rs.getString("dong"),
						rs.getString("floor"), rs.getString("ho"), rs.getDouble("lat"), rs.getDouble("lng"));
				SanggwonDtoList.add(SanggwonDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return SanggwonDtoList;
	}

	@Override
	public List<SanggwonDto> searchByDongcodeCategoryName(String dongCode, String category, String name) {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		List<SanggwonDto> SanggwonDtoList = new ArrayList<SanggwonDto>();
		try {
			conn = DBConnection.getConnection();
			StringBuilder sql = new StringBuilder();
			sql.append("select * " + "from sanggwon where ");
			if (dongCode != null) {
				sql.append("dong_code = ? and ");
			}
			if (category != null) {
				sql.append("category = ? and ");
			}
			if (name != null) {
				sql.append("name like ? and ");
			}
			sql.setLength(sql.length() - 4);
			pstmt = conn.prepareStatement(sql.toString());
			if (dongCode != null) {
				pstmt.setString(1, dongCode);
				if (category != null) {
					pstmt.setString(2, category);
					if (name != null) {
						pstmt.setString(3, "%" + name + "%");
					}
				} else if (category == null && name != null) {
					pstmt.setString(2, name);
				}
			}
			if (dongCode == null) {
				if (category != null) {
					pstmt.setString(1, category);
					if (name != null) {
						pstmt.setString(2, "%" + name + "%");
					}
				} else if (category == null && name != null) {
					pstmt.setString(1, "%" + name + "%");
				}
			}
			rs = pstmt.executeQuery();
			while (rs.next()) {
				SanggwonDto SanggwonDto = new SanggwonDto(rs.getInt("no"), rs.getString("name"), rs.getString("branch"),
						rs.getString("category"), rs.getString("dong_code"), rs.getString("address"),
						rs.getString("building_name"), rs.getString("street_address"), rs.getString("dong"),
						rs.getString("floor"), rs.getString("ho"), rs.getDouble("lat"), rs.getDouble("lng"));
				SanggwonDtoList.add(SanggwonDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			DBClose.close(rs, pstmt, conn);
		}
		return SanggwonDtoList;
	}

}
